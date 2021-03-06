/* ====================================================================
 * Copyright (c) 1997-2000 Carnegie Mellon University.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * This work was supported in part by funding from the Defense Advanced 
 * Research Projects Agency and the National Science Foundation of the 
 * United States of America, and the CMU Sphinx Speech Consortium.
 *
 * THIS SOFTWARE IS PROVIDED BY CARNEGIE MELLON UNIVERSITY ``AS IS'' AND 
 * ANY EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL CARNEGIE MELLON UNIVERSITY
 * NOR ITS EMPLOYEES BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ====================================================================
 *
 */
/*********************************************************************
 *
 * File: dtree.c
 * 
 * Description: 
 * 	State-tying decision tree routines.
 *
 * Author: 
 *     Eric Thayer (eht@cs.cmu.edu)
 *********************************************************************/

#include <s3/dtree.h>
#include <s3/best_q.h>
#include <s3/two_class.h>

#include <s3/ckd_alloc.h>
#include <s3/read_line.h>
#include <s3/quest.h>
#include <s3/div.h>
#include <s3/err.h>

#include <s3/s3.h>
#include <s3/cmd_ln.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>


uint32
cnt_node(dtree_node_t *node)
{
    if (!IS_LEAF(node)) {
	return cnt_node(node->y) + cnt_node(node->n) + 1;
    }
    else {
	return 1;
    }
}

uint32
reindex(dtree_node_t *node,
	uint32 *next_id)
{
    node->node_id = (*next_id)++;

    if (!IS_LEAF(node)) {
	return reindex(node->y, next_id) +
	    reindex(node->n, next_id) + 1;
    }
    else {
	return 1;
    }
}

dtree_node_t *
get_node(dtree_node_t *node,
	 uint32 id)
{
    dtree_node_t *ret;

    if (node->node_id == id)
	return node;
    else if (!IS_LEAF(node)) {
	ret = get_node(node->y, id);
	if (ret != NULL)
	    return ret;
	ret = get_node(node->n, id);
	return ret;
    }
    else {
	/* it is a leaf and it doesn't match */
	return NULL;
    }
}

uint32
prune_lowentred(dtree_node_t *node, float32 entred_thr)
{
    if (!IS_LEAF(node)) {
	/* Not a leaf node */
	if (node->wt_ent < entred_thr) {
	    return prune_subtrees(node);
	}
	else {
	    return
		prune_lowentred(node->y, entred_thr) + prune_lowentred(node->n, entred_thr);
	}
    }
    else {
	return 0;
    }
}

uint32
prune_lowcnt(dtree_node_t *node, float32 cnt_thr)
{
    if (!IS_LEAF(node)) {
	/* Not a leaf node */
	if ((node->y->occ < cnt_thr) ||
	    (node->n->occ < cnt_thr)) {
	    return prune_subtrees(node);
	}
	else {
	    return
		prune_lowcnt(node->y, cnt_thr) + prune_lowcnt(node->n, cnt_thr);
	}
    }
    else {
	return 0;
    }
}

uint32
prune_subtrees(dtree_node_t *node)
{
    uint32 n_pruned;

    if (!IS_LEAF(node)) {
	n_pruned = cnt_node(node->y) + cnt_node(node->n);
	    
	prune_subtrees(node->y); node->y = NULL;
	prune_subtrees(node->n); node->n = NULL;

	return n_pruned;
    }
    else {
	/* it is a leaf node; nothing to do */
	return 0;
    }
}

static void
print_final_tree_recur(FILE *fp,
		       dtree_node_t *node,
		       pset_t *pset)
{
    if (!IS_LEAF(node)) {
	fprintf(fp, "%u %u %u %e %e ",
		node->node_id, node->y->node_id, node->n->node_id, node->wt_ent_dec, node->occ);
	print_comp_quest(fp, pset, (comp_quest_t *)node->q);
	fprintf(fp, "\n");

	print_final_tree_recur(fp, node->y, pset);
	print_final_tree_recur(fp, node->n, pset);
    }
    else {
	fprintf(fp, "%u - - %e %e\n",
		node->node_id, node->wt_ent, node->occ);
    }
}

void
print_final_tree(FILE *fp,
		 dtree_node_t *node,
		 pset_t *pset)
{
    fprintf(fp, "n_node %u\n", cnt_node(node));

    print_final_tree_recur(fp, node, pset);
}

static void
print_final_tree_recur_davinci(FILE *fp,
			       dtree_node_t *node,
			       pset_t *pset,
			       uint32 *lvl)
{
    uint32 i;

    (*lvl)++;

    if (!IS_LEAF(node)) {
	for (i = 0; i < *lvl * 4; i++) {
	    fprintf(fp, " ");
	}
	fprintf(fp, "l(\"%u\",n(\"%u\",[a(\"OBJECT\",\"",
		node->node_id, node->node_id);
	print_comp_quest(fp, pset, (comp_quest_t *)node->q);
	fprintf(fp, "\")],\n");
	for (i = 0; i < *lvl * 4; i++) {
	    fprintf(fp, " ");
	}
	fprintf(fp, "[\n");
	for (i = 0; i < *lvl * 4; i++) {
	    fprintf(fp, " ");
	}
	fprintf(fp, "l(\"%u->%u\",e(\"\",[],\n", node->node_id, node->y->node_id);

	print_final_tree_recur_davinci(fp, node->y, pset, lvl);
	fprintf(fp, ")),\n");
	for (i = 0; i < *lvl * 4; i++) {
	    fprintf(fp, " ");
	}
	fprintf(fp, "l(\"%u->%u\",e(\"\",[],\n", node->node_id, node->n->node_id);
	print_final_tree_recur_davinci(fp, node->n, pset, lvl);
	fprintf(fp, "))]))\n");
    }
    else {
	for (i = 0; i < *lvl * 4; i++) {
	    fprintf(fp, " ");
	}
	fprintf(fp, "l(\"%u\",n(\"%u\",[a(\"OBJECT\",\"%u\")],[]))\n",
		node->node_id, node->node_id, node->clust);
    }

    (*lvl)--;
}

void
print_final_tree_davinci(FILE *fp,
			 dtree_node_t *node,
			 pset_t *pset)
{
    uint32 lvl = 0;
    
    fprintf(fp, "[\n");
    print_final_tree_recur_davinci(fp, node, pset, &lvl);
    fprintf(fp, "]\n");
}

dtree_t *
read_final_tree(FILE *fp,
		pset_t *pset,
		uint32 n_pset)
{
    dtree_t *out;
    dtree_node_t *node;
    uint32  n_node;
    char ln[4096], *s, str[128];
    uint32 n_read, n_scan;
    uint32 i, node_id, node_id_y, node_id_n;
    comp_quest_t *q;
    float64 ent;
    float32 occ;
    int err;

    out = ckd_calloc(1, sizeof(dtree_t));

    n_read = 0;
    read_line(ln, 4096, &n_read, fp);

    s = ln;
    sscanf(s, "%s%n", str, &n_scan);
    if (strcmp(str, "n_node") == 0) {
	s += n_scan;
	sscanf(s, "%u", &n_node);
    }
    else {
	E_FATAL("Format error; expecting n_node\n");
    }

    out->n_node = n_node;
    out->node = node = ckd_calloc(n_node, sizeof(dtree_node_t));

    for (i = 0; i < n_node; i++)
	node[i].node_id = i;
    
    err = FALSE;
    
    while (read_line(ln, 4096, &n_read, fp)) {
	s = ln;

	sscanf(s, "%u%n", &node_id, &n_scan);
	s += n_scan;
	sscanf(s, "%s%n", str, &n_scan);
	s += n_scan;
	if (strcmp(str, "-") == 0) {
	    node_id_y = NO_ID;
	}
	else {
	    node_id_y = atoi(str);
	}
	sscanf(s, "%s%n", str, &n_scan);
	s += n_scan;
	if (strcmp(str, "-") == 0) {
	    node_id_n = NO_ID;
	}
	else {
	    node_id_n = atoi(str);
	}
	sscanf(s, "%le%n", &ent, &n_scan);
	s += n_scan;
	sscanf(s, "%e%n", &occ, &n_scan);
	s += n_scan;

	if ((node_id_y != NO_ID) && (node_id_y != NO_ID)) {
	    q = (comp_quest_t *)ckd_calloc(1, sizeof(comp_quest_t));
	    if (s3parse_comp_quest(pset, n_pset, q, s) != S3_SUCCESS) {
		err = TRUE;
	    }

	    node[node_id].q = q;
	}
	else
	    node[node_id].q = NULL;

	/* ck if internal node */
	if ((node_id_y != NO_ID) && (node_id_y != NO_ID))
	    node[node_id].wt_ent_dec = ent;
	else
	    node[node_id].wt_ent = ent;

	node[node_id].occ = occ;

	if ((node_id_y != NO_ID) && (node_id_y != NO_ID)) {
	    node[node_id].y = &node[node_id_y];
	    node[node_id].n = &node[node_id_n];
	    node[node_id_y].p = node[node_id_n].p = &node[node_id];
	}
	else {
	    node[node_id].y = NULL;
	    node[node_id].n = NULL;
	}
    }

    if (err == TRUE) {
	free_tree(out);
	out = NULL;
    }

    return out;
}


void
free_tree(dtree_t *tr)
{
    uint32 i;
    dtree_node_t *node;

    for (i = 0; i < tr->n_node; i++) {
	node = &tr->node[i];

	if (node->mixw_occ)
	    ckd_free_3d((void ***)node->mixw_occ);
	if (node->means)
	    ckd_free_3d((void ***)node->means);
	if (node->vars)
	    ckd_free_3d((void ***)node->vars);
	if (node->id)
	    ckd_free(node->id);

	/* node->q not freed because if tr is a
	   simple tree, it points to the question
	   in the master list of questions that needs
	   to stick around */
    }

    ckd_free(tr->node);

    ckd_free(tr);
}


void
mk_disj(dtree_node_t *node,
	uint32 clust,
	comp_quest_t *q,
	quest_t *conj,
	uint32 c_idx,
	uint32 *d_idx)
{
    if (IS_LEAF(node)) {
	if (node->clust == clust) {
	    uint32 i;
	    uint32 d;

	    d = *d_idx;
	    q->prod_len[d] = c_idx;
	    q->conj_q[d] = (quest_t *)ckd_calloc(c_idx, sizeof(quest_t));
	    for (i = 0; i < c_idx; i++) {
		memcpy(&q->conj_q[d][i], &conj[i], sizeof(quest_t));
	    }

	    ++(*d_idx);
	}
	else
	    return;
    }
    else {
	quest_t *qu;

	qu = (quest_t *)node->q;

	conj[c_idx].ctxt = qu->ctxt;
	conj[c_idx].neg  = qu->neg;
	conj[c_idx].pset = qu->pset;
	conj[c_idx].member = qu->member;
	conj[c_idx].posn = qu->posn;

	mk_disj(node->y, clust, q, conj, c_idx+1, d_idx);

	conj[c_idx].neg = !conj[c_idx].neg;
	mk_disj(node->n, clust, q, conj, c_idx+1, d_idx);

	return;
    }
}

uint32
cnt_class(dtree_node_t *node,
	  uint32 *n_a,
	  uint32 *n_b)
{
    if (IS_LEAF(node)) {
	if (node->clust)
	    ++(*n_a);
	else
	    ++(*n_b);
    }
    else {
	cnt_class(node->y, n_a, n_b);
	cnt_class(node->n, n_a, n_b);
    }
    return 0;
}

uint32
prune_leaves(dtree_node_t *node,
	     pset_t *pset)
{
    dtree_node_t *y, *n;
    uint32 y_clust, n_clust;

    if (IS_LEAF(node))
	return NO_CLUST;

    y = node->y;
    n = node->n;
    
    if (!IS_LEAF(y)) {
	y_clust = prune_leaves(y, pset);
    }
    else {
	y_clust = y->clust;
    }

    if (!IS_LEAF(n)) {
	n_clust = prune_leaves(n, pset);
    }
    else {
	n_clust = n->clust;
    }


    if ((y_clust != NO_CLUST) && (y_clust == n_clust)) {
	node->y = node->n = NULL;
	node->clust = y_clust;

	return y_clust;
    }

    return NO_CLUST;
}

uint32
leaf_mixw_occ(dtree_node_t *node,
	      pset_t *pset,
	      float32 ****mixw_occ,
	      uint32 *node_id,
	      uint32 n_state,
	      uint32 n_stream,
	      uint32 n_density,
	      uint32 off)
{
    uint32 i, j, k;

    if (IS_LEAF(node)) {
	node_id[off] = node->node_id;

	for (i = 0; i < n_state; i++) {
	    for (j = 0; j < n_stream; j++) {
		for (k = 0; k < n_density; k++) {
		    mixw_occ[off][i][j][k] = node->mixw_occ[i][j][k];
		}
	    }
	}

	return ++off;
    }
    else {
	off = leaf_mixw_occ(node->y,
			pset,
			mixw_occ,
			node_id,
			n_state,
			n_stream,
			n_density,
			off);
	off = leaf_mixw_occ(node->n,
			pset,
			mixw_occ,
			node_id,
			n_state,
			n_stream,
			n_density,
			off);
	return off;
    }
}


/* ADDITION FOR CONTINUOUS_TREES */
uint32
leaf_mean_vars(dtree_node_t *node,
	       pset_t *pset,
	       float32 ****means,
	       float32 ****vars,
	       uint32 *node_id,
	       uint32 n_state,
	       uint32 n_stream,
	       uint32 *veclen,
	       uint32 off)
{
    uint32 i, j, k;

    if (IS_LEAF(node)) {
	node_id[off] = node->node_id;

	for (i = 0; i < n_state; i++) {
	    for (j = 0; j < n_stream; j++) {
		for (k = 0; k < veclen[j]; k++) {
                    means[off][i][j][k] = node->means[i][j][k];
                    vars[off][i][j][k] = node->vars[i][j][k];
		}
	    }
	}

	return ++off;
    }
    else {
	off = leaf_mean_vars(node->y,
			pset,
			means,
                        vars,
			node_id,
			n_state,
			n_stream,
			veclen,
			off);
	off = leaf_mean_vars(node->n,
			pset,
			means,
                        vars,
			node_id,
			n_state,
			n_stream,
			veclen,
			off);
	return off;
    }
}
/* END ADDITION FOR CONTINUOUS_TREES */


uint32
cnt_leaf(dtree_node_t *node)
{
    if ((node->y == NULL) && (node->n == NULL)) {
	return 1;
    }
    else {
	return cnt_leaf(node->y) + cnt_leaf(node->n);
    }
}

uint32
label_leaves(dtree_node_t *node, uint32 *id)
{
    if (IS_LEAF(node)) {
	node->clust = (*id)++;

	return 1;
    }
    else {
	return label_leaves(node->y, id) + label_leaves(node->n, id);
    }
}
uint32
tied_state(dtree_node_t *node,
	   acmod_id_t b,
	   acmod_id_t l,
	   acmod_id_t r,
	   word_posn_t wp,
	   pset_t *pset)
{
    uint32 dfeat[4];

    dfeat[0] = (uint32)l;
    dfeat[1] = (uint32)b;
    dfeat[2] = (uint32)r;
    dfeat[3] = (uint32)wp;

    if (IS_LEAF(node)) {
	/* Got to a state cluster.  Return the id */
	return node->clust;
    }
    else {
#ifdef HORRIBLY_VERBOSE
	print_comp_quest(stderr, pset, (comp_quest_t *)node->q);
#endif
	if (eval_comp_quest((comp_quest_t *)node->q, dfeat, 4)) {
#ifdef HORRIBLY_VERBOSE
	    fprintf(stderr, " -> y\n");
#endif
	    return tied_state(node->y, b, l, r, wp, pset);
	}
	else {
#ifdef HORRIBLY_VERBOSE
	    fprintf(stderr, " -> n\n");
#endif
	    return tied_state(node->n, b, l, r, wp, pset);
	}
    }
}

/*
 * A "twig" is a node where both children are leaves
 */
uint32
cnt_twig(dtree_node_t *node)
{
    if (IS_LEAF(node)) {
	return 0;
    } else if (IS_LEAF(node->y) && IS_LEAF(node->n)) {
	return 1;
    }
    else {
	if (IS_LEAF(node->y) && !IS_LEAF(node->n)) {
	    return cnt_twig(node->n);
	}
	else if (!IS_LEAF(node->y) && IS_LEAF(node->n)) {
	    return cnt_twig(node->y);
	}
	else 
	    return cnt_twig(node->y) + cnt_twig(node->n);
    }
}

void
print_node(FILE *fp, dtree_node_t *node, pset_t *pset)
{
    if (!IS_LEAF(node)) {
	print_quest(fp, pset, (quest_t *)node->q);
	fprintf(fp, " %.3e %.3e %.3e",
/* DDDDBUG chk */
		node->wt_ent, node->wt_ent_dec,node->occ);
/**/
    }
    else {
	fprintf(fp, "-");
	fprintf(fp, " %.3e %u %u", node->wt_ent, node->n_id, node->clust);
	if (node->q) {
	    fprintf(fp, " ");
	    print_quest(fp, pset, (quest_t *)node->q);
	    fprintf(fp, " %.3e", node->wt_ent_dec);
/* DDDBUG for checks ..*/
	    fprintf(fp, " %.3e",node->occ);
/**/
	}
    }
}

void
print_node_comp(FILE *fp, dtree_node_t *node, pset_t *pset)
{
    if (!IS_LEAF(node)) {
	print_comp_quest(fp, pset, (comp_quest_t *)node->q);
	fprintf(fp, " %.3e %.3e",
		node->wt_ent, node->wt_ent_dec);
    }
    else {
	fprintf(fp, "-");
	fprintf(fp, " %.3e [%u]", node->wt_ent, node->n_id);
	if (node->q) {
	    fprintf(fp, " ");
	    print_comp_quest(fp, pset, (comp_quest_t *)node->q);
	    fprintf(fp, " %.3e", node->wt_ent_dec);
	}
    }
}

void
print_tree(FILE *fp,
	   char *label,
	   dtree_node_t *node,
	   pset_t *pset,
	   uint32 lvl)
{
    char i;

    fprintf(fp, "%s", label);

    for (i = 0; i < lvl*4; i++) {
	putc(' ', fp);
    }

    fprintf(fp, "( ");

    print_node(fp, node, pset);

    if (node->y && node->n) {
	fprintf(fp, "\n");
	print_tree(fp, label, node->y, pset, lvl+1);

	fprintf(fp, "\n");
	print_tree(fp, label, node->n, pset, lvl+1);
    }

    fprintf(fp, ")");
}


void
print_tree_comp(FILE *fp,
		char *label,
		dtree_node_t *node,
		pset_t *pset,
		uint32 lvl)
{
    char i;

    fprintf(fp, "%s", label);
    for (i = 0; i < lvl*4; i++) {
	putc(' ', fp);
    }

    fprintf(fp, "( ");

    print_node_comp(fp, node, pset);

    if (node->y && node->n) {
	fprintf(fp, "\n");
	print_tree_comp(fp, label, node->y, pset, lvl+1);

	fprintf(fp, "\n");
	print_tree_comp(fp, label, node->n, pset, lvl+1);
    }

    fprintf(fp, ")");
}


int
mk_node(dtree_node_t *node,
	uint32 node_id,
	uint32 *id,
	uint32 n_id,
	float32 ****mixw,
/* ADDITION FOR CONTINUOUS_TREES, 18 May 98 */
        float32 ****means,
        float32 ****vars,
        uint32  *veclen,
/* END ADDITION FOR CONTINUOUS_TREES */
	uint32 n_model,
	uint32 n_state,
	uint32 n_stream,
	uint32 n_density,
	float32 *stwt,
	float32 mwfloor)
{
    float32 ***mixw_occ, **dist;
    uint32 mm, m, s, j, k;
    float64 *dnom, norm, wt_ent, s_wt_ent, occ;
    float32 mx_wt;
    uint32 *l_id;
/* ADDITION FOR CONTINUOUS_TREES 19 May 98*/
    float32 ***lmeans=0,***lvars=0;
    float32 varfloor=0;
    uint32 continuous, sumveclen;
    char*  type;

    type = (char *)cmd_ln_access("-ts2cbfn");
    if (strcmp(type,".semi.")!=0 && strcmp(type,".cont.") != 0)
        E_FATAL("Type %s unsupported; trees can only be built on types .semi. or .cont.\n",type);
    if (strcmp(type,".cont.") == 0) 
        continuous = 1;
    else 
        continuous = 0;

    if (continuous == 1) {
        varfloor = *(float32 *)cmd_ln_access("-varfloor");
        /* Sumveclen is overallocation, but coding is simpler */
        for (j=0,sumveclen=0; j < n_stream; j++) sumveclen += veclen[j];

        lmeans   = (float32 ***) ckd_calloc_3d(n_state,n_stream,sumveclen,sizeof(float32));
        lvars   = (float32 ***) ckd_calloc_3d(n_state,n_stream,sumveclen,sizeof(float32));
    }
/* END ADDITION FOR CONTINUOUS_TREES */

    mixw_occ = (float32 ***)ckd_calloc_3d(n_state, n_stream, n_density, sizeof(float32));
    dist     = (float32 **)ckd_calloc_2d(n_stream, n_density, sizeof(float32));
    dnom     = (float64 *)ckd_calloc(n_stream, sizeof(float64));

/* MODIFICATION/ADDITIONS FOR CONTINUOUS_TREES; 19 May 98: merge means and vars too */

    /* Merge distributions of all the elements in a cluster for combined
       distribution */
    for (s = 0; s < n_state; s++) {
	for (j = 0; j < n_stream; j++) {
            float32 *lmeanvec=0, *lvarvec=0;
            if (continuous == 1) {
                lmeanvec = lmeans[s][j];
                lvarvec = lvars[s][j];
            }
	    for (mm = 0; mm < n_id; mm++) {
	        m = id[mm];
		for (k = 0; k < n_density; k++) {
		    mixw_occ[s][j][k] += mixw[m][s][j][k];
		}
                /* For continuous hmms we have only one gaussian per state */
                if (continuous == 1) {
                    for (k = 0; k < veclen[j]; k++) {
                        lmeanvec[k] += mixw[m][s][j][0] * means[m][s][j][k];
                        lvarvec[k] += mixw[m][s][j][0] * (vars[m][s][j][k] + 
                                        means[m][s][j][k] * means[m][s][j][k]);
	            }
	        }
	    }
            if (continuous == 1) {
                if (mixw_occ[s][j][0] != 0) {
                    for (k = 0; k < veclen[j]; k++) {
                        lmeanvec[k] /= mixw_occ[s][j][0];
                        lvarvec[k] = lvarvec[k]/mixw_occ[s][j][0] - 
                                            lmeanvec[k]*lmeanvec[k];
                        if (lvarvec[k] < varfloor) lvarvec[k] = varfloor;
	            }
	        }
                else {
                    for (k = 0; k < veclen[j]; k++) 
                        if (lmeanvec[k] != 0)
                            E_FATAL("denominator = 0, but numerator = %f at k = %d\n",lmeanvec[k],k);
                }
	    }
	}
    }
/* END ADDITIONS FOR CONTINUOUS_TREES */

    /* Find out which state is under consideration */
    for (j = 0, mx_wt = 0, s = 0; s < n_state; s++) {
	if (stwt[s] > mx_wt) {
	    mx_wt = stwt[s];
	    j = s;
	}
    }

    /* occ is the same for each independent feature, so just choose 0 */
    for (k = 0, occ = 0; k < n_density; k++) {
	occ += mixw_occ[j][0][k];
    }

/* ADDITION/MODIFICATION FOR CONTINUOUS_TREES 19 May 98: USE MEANS AND VARIANCES
   IN THE CASE OF CONTINUOUS HMMs (RATHER THAN MIXWs) */
    for (s = 0, wt_ent = 0; s < n_state; s++) {
	for (j = 0; j < n_stream; j++) {
	    for (k = 0, dnom[j] = 0; k < n_density; k++) {
	        dnom[j] += mixw_occ[s][j][k];
	    }
	}

	for (j = 0, s_wt_ent = 0; j < n_stream; j++) {
	    norm = 1.0 / dnom[j];

            /* discrete_entropy for discrete case, continuous entropy for
               continuous HMMs */
            if (continuous != 1) {
	        for (k = 0; k < n_density; k++) {
	    	    dist[j][k] = mixw_occ[s][j][k] * norm;
		    if (dist[j][k] < mwfloor)
		        dist[j][k] = mwfloor;
	        }

	        s_wt_ent += dnom[j] * ent_d(dist[j], n_density);
            }
            else {
	        s_wt_ent += dnom[j] * ent_cont(lmeans[s][j], lvars[s][j], veclen[j]);
            }
	}

	wt_ent += stwt[s] * s_wt_ent;
    }
/* END MODIFICATIONS / ADDITION FOR CONTINUOUS_TREES */

    node->node_id = node_id;
    l_id = ckd_calloc(n_id, sizeof(uint32));
    for (j = 0; j < n_id; j++) {
	l_id[j] = id[j];
    }
    node->id = l_id;
    node->n_id = n_id;
    node->mixw_occ = mixw_occ;
/* ADDITION FOR CONTINUOUS_TREES, 19 May 98 */
    if (continuous == 1) {
        node->means = lmeans;
        node->vars = lvars;
    }
/* END ADDITION FOR CONTINUOUS_TREES */
    node->occ = occ;
    node->wt_ent = wt_ent;

    ckd_free_2d((void **)dist);
    ckd_free((void *)dnom);

    return S3_SUCCESS;
}


float64
set_best_quest(dtree_node_t *node,
	       float32 ****mixw,
/* ADDITION FOR CONTINUOUS_TREES, 20 May 98 */
               float32 ****means,
               float32 ****vars,
               uint32  *veclen,
/* END ADDITION FOR CONTINUOUS_TREES */
	       uint32 n_model,
	       uint32 n_state,
	       uint32 n_stream,
	       uint32 n_density,
	       float32 *stwt,

	       quest_t *all_q,
	       uint32 n_all_q,
	       pset_t *pset,

	       uint32 **dfeat,
	       uint32 n_dfeat,

	       float32 mwfloor)
{
    float32 ***dist;
    float64 norm;
    uint32 s, j, k;
    
    dist = (float32 ***)ckd_calloc_3d(n_state, n_stream, n_density, sizeof(float32));

/* COMMENT ADDED FOR CONTINUOUS_TREES */
    /* Convert occ. counts to probabilities. norm now has total occ. count */
/* END ADDITION FOR CONTINUOUS_TREES */
    for (s = 0; s < n_state; s++) {
	for (j = 0; j < n_stream; j++) {
	    for (k = 0; k < n_density; k++) {
		dist[s][j][k] = node->mixw_occ[s][j][k];
	    }
	}
	for (k = 0, norm = 0; k < n_density; k++) {
	    norm += dist[s][0][k];
	}
	norm = 1.0 / norm;
	for (j = 0; j < n_stream; j++) {
	    for (k = 0; k < n_density; k++) {
		dist[s][j][k] *= norm;
		if (dist[s][j][k] < mwfloor) {
		    dist[s][j][k] = mwfloor;
		}
	    }
	}
    }

/* MODIFICATION FOR CONTINUOUS_TREES; PASS MEAN, VAR, VECLEN AND THE WEIGHTED ENTROPY 
   OF THE NODE ALSO */
    node->wt_ent_dec = best_q(mixw, means, vars, veclen,
                              n_model, n_state, n_stream, n_density,
			      stwt,
			      dfeat, n_dfeat,
			      all_q, n_all_q,
			      pset,
			      node->id, node->n_id,
			      dist, node->wt_ent,
			      (quest_t **)&node->q);
/* END MODIFICATION FOR CONTINUOUS_TREES */

    ckd_free_3d((void ***)dist);

    return node->wt_ent_dec;
}


void
split_node_comp(dtree_t *tr,
		uint32 node_id,
		float32 ****mixw,
/* ADDITION FOR CONTINUOUS_TREES */
                float32 ****means,
                float32 ****vars,
                uint32  *veclen,
/* END ADDITION FOR CONTINUOUS_TREES */
		uint32 n_model,
		uint32 n_state,
		uint32 n_stream,
		uint32 n_density,
		float32 *stwt,
		quest_t *all_q,
		uint32 n_all_q,
		pset_t *pset,
		uint32 n_base_phone,
		uint32 **dfeat,
		uint32 n_dfeat,
		uint32  split_min,
		uint32  split_max,
		float32 split_thr,
		float32 mwfloor)
{
    uint32 *id, n_id;
    uint32 *id_yes, n_yes;
    uint32 *id_no, n_no;
    dtree_node_t *node;
    uint32 node_id_yes;
    uint32 node_id_no;
    uint32 ii, i;

    node = &tr->node[node_id];
    id = node->id;
    n_id = node->n_id;

    for (ii = 0, n_yes = 0, n_no = 0; ii < n_id; ii++) {
	i = id[ii];

	if (eval_comp_quest((comp_quest_t *)node->q, dfeat[i], n_dfeat)) {
	    ++n_yes;
	}
	else {
	    ++n_no;
	}
    }

#if 0
    fprintf(stderr, "Comp Split: ");
    print_comp_quest(stderr, pset, (comp_quest_t *)node->q);
    fprintf(stderr, " %u/%u %.3e\n", n_yes, n_no, node->wt_ent_dec);
#endif
    
    id_yes = ckd_calloc(n_yes, sizeof(uint32));
    id_no = ckd_calloc(n_no, sizeof(uint32));

    for (ii = 0, n_yes = 0, n_no = 0; ii < n_id; ii++) {
	i = id[ii];

	if (eval_comp_quest((comp_quest_t *)node->q, dfeat[i], n_dfeat)) {
	    id_yes[n_yes] = i;
	    ++n_yes;
	}
	else {
	    id_no[n_no] = i;
	    ++n_no;
	}
    }

    node_id_yes = tr->n_node++;
    node_id_no = tr->n_node++;

    node->y = &tr->node[node_id_yes];
    node->n = &tr->node[node_id_no];
    node->y->p = node;
    node->n->p = node;

/* MODIFICATION FOR CONTINUOUS_TREES; Additions for Continuous Hmm, pass means, vars,
   veclen */
    mk_node(node->y,
	    node_id_yes,
	    id_yes, n_yes,
	    mixw, means, vars, veclen,
            n_model, n_state, n_stream, n_density, stwt, mwfloor);

    node->y->q = (void *)mk_comp_quest(&(node->y->wt_ent_dec),
				       mixw, means, vars, veclen,
                                       n_model, n_state, n_stream, n_density, 
                                       stwt, id_yes, n_yes,
				       all_q, n_all_q, pset, n_base_phone,
				       dfeat, n_dfeat,
				       split_min, split_max, split_thr, mwfloor);

    mk_node(node->n,
	    node_id_no,
	    id_no, n_no,
	    mixw, means, vars, veclen,
            n_model, n_state, n_stream, n_density, stwt, mwfloor);

    node->n->q = (void *)mk_comp_quest(&(node->n->wt_ent_dec),
				       mixw, means, vars, veclen, 
                                       n_model, n_state, n_stream, n_density, 
                                       stwt, id_no, n_no,
				       all_q, n_all_q, pset, n_base_phone,
				       dfeat, n_dfeat,
				       split_min, split_max, split_thr, mwfloor);
/* END MODIFICATIONS FOR CONTINUOUS_TREES */
}


void
split_node(dtree_t *tr,
	   uint32 node_id,
	   float32 ****mixw,
/* ADDITION FOR CONTINUOUS_TREES */
           float32 ****means,
           float32 ****vars,
           uint32 *veclen,
/* END ADDITION FOR CONTINUOUS_TREES */
	   uint32 n_model,
	   uint32 n_state,
	   uint32 n_stream,
	   uint32 n_density,
	   float32 *stwt,

	   quest_t *all_q,
	   uint32 n_all_q,
	   pset_t *pset,
	   
	   uint32 **dfeat,
	   uint32 n_dfeat,

	   float32 mwfloor)
{
    uint32 *id, n_id;
    uint32 *id_yes, n_yes;
    uint32 *id_no, n_no;
    dtree_node_t *node;
    uint32 node_id_yes;
    uint32 node_id_no;
    uint32 ii, i;

    node = &tr->node[node_id];

    id = node->id;
    n_id = node->n_id;

    for (ii = 0, n_yes = 0, n_no = 0; ii < n_id; ii++) {
	i = id[ii];

	if (eval_quest((quest_t *)node->q, dfeat[i], n_dfeat)) {
	    ++n_yes;
	}
	else {
	    ++n_no;
	}
    }

#if 0
    fprintf(stderr, "Split: (");
    print_quest(stderr, pset, (quest_t *)node->q);
    fprintf(stderr, ") %u/%u %.3e\n", n_yes, n_no, node->wt_ent_dec);
#endif

    id_yes = ckd_calloc(n_yes, sizeof(uint32));
    id_no = ckd_calloc(n_no, sizeof(uint32));

    for (ii = 0, n_yes = 0, n_no = 0; ii < n_id; ii++) {
	i = id[ii];

	if (eval_quest((quest_t *)node->q, dfeat[i], n_dfeat)) {
	    id_yes[n_yes] = i;
	    ++n_yes;
	}
	else {
	    id_no[n_no] = i;
	    ++n_no;
	}
    }

    node_id_yes = tr->n_node++;
    node_id_no = tr->n_node++;

    node->y = &tr->node[node_id_yes];
    node->n = &tr->node[node_id_no];
    node->y->p = node;
    node->n->p = node;

/* MODIFICATION FOR CONTINUOUS_TREES; Pass means, vars and veclen also */
    mk_node(node->y,
	    node_id_yes,
	    id_yes, n_yes,
	    mixw, means, vars, veclen,
            n_model, n_state, n_stream, n_density, stwt, mwfloor);

    set_best_quest(node->y,
		   mixw,
                   means,
                   vars,
                   veclen,
		   n_model,
		   n_state,
		   n_stream,
		   n_density,
		   stwt,
		   all_q, n_all_q, pset,
		   dfeat, n_dfeat,
		   mwfloor);

    mk_node(node->n,
	    node_id_no,
	    id_no, n_no,
	    mixw, means, vars, veclen,
            n_model, n_state, n_stream, n_density, stwt, mwfloor);

    set_best_quest(node->n,
		   mixw,
                   means,
                   vars,
                   veclen,
		   n_model,
		   n_state,
		   n_stream,
		   n_density,
		   stwt,
		   all_q, n_all_q, pset,
		   dfeat, n_dfeat,
		   mwfloor);
/* END MODIFICATION FOR CONTINUOUS_TREES */
}


dtree_node_t *
best_leaf_node(dtree_node_t *node)
{
    dtree_node_t *cmp_y, *cmp_n, *ret;

    ret = NULL;

    if (IS_LEAF(node)) {
	ret = node;
    }
    else {
	cmp_y = best_leaf_node(node->y);
	cmp_n = best_leaf_node(node->n);

	if ((cmp_y == NULL || cmp_y->q == NULL) &&
	    (cmp_n == NULL || cmp_n->q == NULL)) {
	    return NULL;
	}
	else if ((cmp_y == NULL) || (cmp_y->q == NULL)) {
	    ret = cmp_n;
	}
	else if ((cmp_n == NULL) || (cmp_n->q == NULL)) {
	    ret = cmp_y;
	}
	else if (cmp_y->wt_ent_dec > cmp_n->wt_ent_dec) {
	    ret = cmp_y;
	}
	else {
	    ret = cmp_n;
	}
    }

    return ret;
}

dtree_t *
mk_tree_comp(float32 ****mixw,
/* ADDITION FOR CONTINUOUS_TREES, 18 May 98 */
             float32 ****means, 
             float32 ****vars, 
             uint32  *veclen,
/* END ADDITIONS FOR CONTINUOUS_TREES */
	     uint32 n_model,
	     uint32 n_state,
	     uint32 n_stream,
	     uint32 n_density,
	     float32 *stwt,
	
	     uint32 *id,
	     uint32 n_id,

	     quest_t *all_q,
	     uint32 n_all_q,
	     pset_t *pset,
	     uint32 n_base_phone,
	     
	     uint32 **dfeat,
	     uint32 n_dfeat,

	     uint32  split_min,
	     uint32  split_max,
	     float32 split_thr,

	     uint32  split_min_comp,
	     uint32  split_max_comp,
	     float32 split_thr_comp,

	     float32 mwfloor)
{
    dtree_t *comp_tree;
    dtree_node_t *root, *b_n;
    uint32 i;

    comp_tree = ckd_calloc(1, sizeof(dtree_t));
    comp_tree->node = ckd_calloc(2*split_max_comp+1, sizeof(dtree_node_t));
    comp_tree->n_node = 0;

    comp_tree->node[0].node_id = 0;
    comp_tree->n_node = 1;
    
    root = &comp_tree->node[0];

/* MODIFICATION FOR CONTINUOUS_TREES 18 May 98, pass means and var along with mixw */
    mk_node(root, 0,
	    id, n_id,
	    mixw, means, vars, veclen,
            n_model, n_state, n_stream, n_density, stwt, mwfloor);
/* END MODIFICATION FOR CONTINUOUS_TREES */

/* MODIFICATION FOR CONTINUOUS_TREES 18 May 98, pass means and var along with mixw */
    root->q = (void *)mk_comp_quest(&root->wt_ent_dec,
				    mixw, means, vars, veclen, 
                                    n_model, n_state, n_stream, n_density, stwt,
				    id, n_id,
				    all_q, n_all_q, pset, n_base_phone,
				    dfeat, n_dfeat,
				    split_min, split_max, split_thr,
				    mwfloor);
/* END MODIFICATION FOR CONTINUOUS_TREES */
    
    for (i = 0; i < split_max_comp; i++) {
	b_n = best_leaf_node(root);

	E_INFO("Comp split %u\n", i);

	if (b_n == NULL) {
	    E_INFO("stop. leaf nodes are specific\n");

	    break;
	}

	if (b_n->wt_ent_dec <= 0) {
	    E_INFO("stop. b_n->wt_ent_dec (%.3e) <= 0\n",
		   b_n->wt_ent_dec);
	    break;
	}
	if ((i > split_min_comp) &&
	    (b_n->wt_ent_dec < split_thr_comp * b_n->wt_ent)) {
	    E_INFO("stop. b_n->wt_ent_dec <= split_thr_comp * b_n->wt_ent. %.3e <= %.3e\n",
		   b_n->wt_ent_dec, split_thr_comp * b_n->wt_ent);
	    break;
	}
		   
/* MODIFICATION FOR CONTINUOUS_TREES 18 May 98, pass means and var along with mixw */
	split_node_comp(comp_tree, b_n->node_id,
			mixw, means, vars, veclen, 
                        n_model, n_state, n_stream, n_density, stwt,
			all_q, n_all_q, pset, n_base_phone,
			dfeat, n_dfeat,
			split_min, split_max, split_thr, mwfloor);
/* END MODIFICATION FOR CONTINUOUS_TREES */

#if 0
	printf("Comp Split %u:\n", i);
	print_tree_comp(stderr, "*", root, pset, 0);
	fprintf(stderr, "\n");
#endif
    }

#if 0
    E_INFO("Final Comp Tree %u:\n", i);
    print_tree_comp(stderr, "", root, pset, 0);
    fprintf(stderr, "\n");
#endif

    return comp_tree;
}

dtree_t *
mk_tree(float32 ****mixw,
/* ADDITION FOR CONTINUOUS_TREES, 20 May 98 */
        float32 ****means,
        float32 ****vars,
        uint32  *veclen,
/* END ADDITION FOR CONTINUOUS_TREES */
	uint32 n_model,
	uint32 n_state,
	uint32 n_stream,
	uint32 n_density,
	float32 *stwt,
	
	uint32 *id,
	uint32 n_id,

	quest_t *all_q,
	uint32 n_all_q,
	pset_t *pset,

	uint32 **dfeat,
	uint32 n_dfeat,

	uint32 split_min,
	uint32 split_max,
	float32 split_thr,

	float32 mwfloor)
{
    dtree_t *s_tree;
    uint32 i;
    dtree_node_t *b_n, *root;

    s_tree = ckd_calloc(1, sizeof(dtree_t));
    s_tree->node = ckd_calloc(2*split_max + 1, sizeof(dtree_node_t));
    s_tree->n_node = 0;

    s_tree->node[0].node_id = 0;
    s_tree->n_node = 1;

    root = &s_tree->node[0];

    mk_node(root, 0,
	    id, n_id,
	    mixw,
/* ADDITION FOR CONTINUOUS_TREES; 20 May 98; passing means, vars, veclen */
            means,
            vars,
            veclen,
/* END ADDITION FOR CONTINUOUS_TREES */
	    n_model,
	    n_state,
	    n_stream,
	    n_density,
	    stwt,
	    mwfloor);

    set_best_quest(root,
		   mixw,
/* ADDITION FOR CONTINUOUS_TREES; 20 May 98; passing means, vars and veclen */
                   means,
                   vars,
                   veclen,
/* END ADDITION FOR CONTINUOUS_TREES */
		   n_model,
		   n_state,
		   n_stream,
		   n_density,
		   stwt,
		   all_q,
		   n_all_q,
		   pset,
		   dfeat, n_dfeat,
		   mwfloor);

    if (root->q == NULL) {
	/* No question found that is able to split node;
	   can't go any further */
	free_tree(s_tree);

	return NULL;
    }

    for (i = 0; i < split_max; i++) {
	b_n = best_leaf_node(root);

	if (b_n == NULL) {
	    E_INFO("stop. leaf nodes are specific\n");

	    break;
	}

/* DDDDDBUG The following criteria will fail if we use only likelihood and no
    likelihood increase */

	if (b_n->wt_ent_dec <= 0) {
	    E_INFO("stop. b_n->wt_ent_dec (%.3e) <= 0\n",
		   b_n->wt_ent_dec);
	    break;
	}

	if ((i > split_min) &&
	    (b_n->wt_ent_dec < split_thr * b_n->wt_ent)) {

	    E_INFO("stop. b_n->wt_ent_dec (%.3e) < split_thr * b_n->wt_ent (%.3e)\n",
		   b_n->wt_ent_dec, b_n->wt_ent * split_thr);

	    break;
	}

	split_node(s_tree, b_n->node_id,
		   mixw,
/* ADDITION FOR CONTINUOUS_TREES; 20 May 98; passing means, vars and veclen */
                   means,
                   vars,
                   veclen,
/* END ADDITION FOR CONTINUOUS_TREES */
		   n_model,
		   n_state,
		   n_stream,
		   n_density,
		   stwt,
		   all_q, n_all_q, pset,
		   dfeat, n_dfeat,
		   mwfloor);
    }

#if 1
    E_INFO("Final simple tree\n");
    print_tree(stderr, "|", root, pset, 0);
    fprintf(stderr, "\n");
#endif

    return s_tree;
}


comp_quest_t *
tree2quest(dtree_t *tr,
	   pset_t *pset,
	   uint32 split_max,
	   uint32 clust,
	   uint32 disj_len)
{
    quest_t *tmp_conj;
    comp_quest_t *q;
    uint32 d_idx;

    tmp_conj = (quest_t *)ckd_calloc(split_max, sizeof(quest_t));

    q = (comp_quest_t *)ckd_calloc(1, sizeof(comp_quest_t));

    q->sum_len = disj_len;
    q->prod_len = ckd_calloc(disj_len, sizeof(uint32));
    q->conj_q = ckd_calloc(disj_len, sizeof(quest_t *));

    d_idx = 0;
    mk_disj(&tr->node[0], clust, q, tmp_conj, 0, &d_idx);

    ckd_free(tmp_conj);

    return q;
}


void
cluster_leaves(dtree_t *tr,
/* ADDITION FOR CONTINUOUS_TREES */
               uint32 *veclen,
/* END ADDITION FOR CONTINUOUS_TREES */
	       float64 *wt_ent_dec,
	       uint32 *out_n_a,
	       uint32 *out_n_b,
	       pset_t *pset,
	       uint32 n_state,
	       uint32 n_stream,
	       uint32 n_density,
	       float32 *stwt,
	       float32 mwfloor)
{
    uint32 n_leaf;
    float32 ****mixw_occ;
    uint32 *clust, n_a, n_b;
    uint32 *node_id;
    dtree_node_t *root;
    uint32 i;
/* ADDITION FOR CONTINUOUS_TREES */
    float32 ****means=0;
    float32 ****vars=0;
    char*  type;
    uint32 continuous, sumveclen;

    type = (char *)cmd_ln_access("-ts2cbfn");
    if (strcmp(type,".semi.")!=0 && strcmp(type,".cont.") != 0)
        E_FATAL("Type %s unsupported; trees can only be built on types .semi. or .cont.\n",type);
    if (strcmp(type,".cont.") == 0)
        continuous = 1;
    else
        continuous = 0;

/* END ADDITIONS FOR CONTINUOUS_TREES */

    root = &tr->node[0];

    /* determine the # of leaf nodes in the simple tree */
    n_leaf = cnt_leaf(root);

/* ADDITION FOR CONTINUOUS_TREES */
    if (continuous == 1) {
        for (i=0,sumveclen=0; i < n_stream; i++) sumveclen += veclen[i];
        means = (float32 ****)ckd_calloc_4d(n_leaf, n_state, n_stream, sumveclen, sizeof(float32));
        vars = (float32 ****)ckd_calloc_4d(n_leaf, n_state, n_stream, sumveclen, sizeof(float32));
    }
/* END ADDITIONS FOR CONTINUOUS_TREES */

    /* Alloc space for:
     *  - leaf node distribution array
     *  - leaf node cluster id array
     *  - leaf node id array
     */
    mixw_occ = (float32 ****)ckd_calloc_4d(n_leaf, n_state, n_stream, n_density, sizeof(float32));
    clust = (uint32 *)ckd_calloc(n_leaf, sizeof(uint32));
    node_id = (uint32 *)ckd_calloc(n_leaf, sizeof(uint32));

    /* compute the density occupancies of the leaves */
    leaf_mixw_occ(root, pset, mixw_occ, node_id, n_state, n_stream, n_density, 0);
/* MODIFICATION FOR CONTINUOUS_TREES; ACT ACCORDING TO TYPE */
    if (continuous == 1) {
        /* compute means and variances of the leaves */
        leaf_mean_vars(root, pset, means, vars, node_id, n_state, n_stream, veclen, 0);
    }
/* END MODIFICATION FOR CONTINUOUS_TREES */

/* MODIFICATION FOR CONTINUOUS_TREES; Pass means, vars and veclen also */
    /* Cluster the leaf nodes into two classes */
    *wt_ent_dec = two_class(mixw_occ, means, vars, veclen, n_leaf, n_state, n_stream, n_density, stwt, clust, mwfloor);
/* END MODIFICATION FOR CONTINUOUS_TREES */

    for (i = 0; i < n_leaf; i++) {
	tr->node[node_id[i]].clust = clust[i];
    }

    /* Simplify the tree based on the two classes
    * (i.e. if siblings belong to the same class,
    * delete the node) */
    prune_leaves(root, pset);

    /* Determine how many leaf nodes in class A and B
     * in the simplified tree */
    n_a = n_b = 0;
    cnt_class(root, &n_a, &n_b);

#if 0
    fprintf(stderr, "Pruned tree %u/%u:\n", n_a, n_b);
    print_tree(stderr, "|", root, pset, 1);
    fprintf(stderr, "\n");
#endif

    *out_n_a = n_a;
    *out_n_b = n_b;
}


comp_quest_t *
mk_comp_quest(float64 *wt_ent_dec,
	      float32 ****mixw,
/* ADDITIONS FOR CONTINUOUS_TREES, 19 May 98 */
              float32 ****means,
              float32 ****vars,
              uint32  *veclen,
 /* END ADDITIONS FOR CONTINUOUS_TREES */
	      uint32 n_model,
	      uint32 n_state,
	      uint32 n_stream,
	      uint32 n_density,
	      float32 *stwt,
	
	      uint32 *id,
	      uint32 n_id,

	      quest_t *all_q,
	      uint32 n_all_q,
	      pset_t *pset,
	      uint32 n_base_phone,

	      uint32 **dfeat,
	      uint32 n_dfeat,
	      
	      uint32 split_min,
	      uint32 split_max,
	      float32 split_thr,

	      float32 mwfloor)
{
    dtree_t *tr;
    uint32 n_a, n_b;
    comp_quest_t *q;

/* MODIFICATION FOR CONTINUOUS_TREES, 20 May 98; Pass mean, var and varlen also */
    tr = mk_tree(mixw, means, vars, veclen, 
                 n_model, n_state, n_stream, n_density, stwt,
		 id, n_id,
		 all_q, n_all_q, pset,
		 dfeat, n_dfeat,
		 split_min, split_max, split_thr,
		 mwfloor);
/* END MODIFICATION FOR CONTINUOUS_TREES */

    if (tr == NULL) {
	*wt_ent_dec = 0;
	return NULL;
    }

    print_tree(stderr, "s>", &tr->node[0], pset, 1);

/* COMMENT FOR CONTINUOUS_TREES, 20 May 89 */
    /* Note that the tree now contains both mixw and mean and var info
       so they no longer need to be passed */
/* END COMMENT FOR CONTINUOUS_TREES */
/* MODIFICATION FOR CONTINUOUS_TREES,20 May 98; Pass veclen; tree already has mean and var */
    cluster_leaves(tr, veclen, wt_ent_dec, &n_a, &n_b,
		   pset,
		   n_state, n_stream, n_density, stwt,
		   mwfloor);
/* END MODIFICATION FOR CONTINUOUS_TREES */

    /* Build the disjunction w/ the fewest terms */
    if (n_b > n_a) {
	q = tree2quest(tr, pset, split_max, 1, n_a);
    }
    else {
	q = tree2quest(tr, pset, split_max, 0, n_b);
    }

    free_tree(tr);

    if (simplify_comp_quest(q, n_base_phone)) {
    }

    return q;
}


#include <s3/heap.h>

int
ins_twigs(dtree_node_t *node,
	  uint32 phnid,
	  uint32 state,
	  float32 *twig_heap,
	  uint32 *twig_hkey,
	  uint32 *phnidlst,
	  uint32 *statelst,
	  uint32 *nidlst,
	  uint32 *free_key)
{
    if (IS_LEAF(node)) {
	return S3_SUCCESS;
    }
    else if (IS_LEAF(node->y) && IS_LEAF(node->n)) {
	uint32 fk;

	fk = *free_key;

	phnidlst[fk] = phnid;
	statelst[fk] = state;
	nidlst[fk] = node->node_id;
	
	heap32b_ins(twig_heap, twig_hkey, fk,
		    node->wt_ent_dec, fk,
		    heap32b_min_comp);

	(*free_key)++;

	return S3_SUCCESS;
    }
    else if (IS_LEAF(node->y) && !IS_LEAF(node->n)) {
	return ins_twigs(node->n,
			 phnid, state,
			 twig_heap, twig_hkey,
			 phnidlst, statelst, nidlst,
			 free_key);
    }
    else if (!IS_LEAF(node->y) && IS_LEAF(node->n)) {
	return ins_twigs(node->y,
			 phnid,
			 state,
			 twig_heap, twig_hkey,
			 phnidlst, statelst, nidlst,
			 free_key);

    }
    else {
	ins_twigs(node->y,
		  phnid, state,
		  twig_heap, twig_hkey,
		  phnidlst, statelst, nidlst,
		  free_key);
	
	return ins_twigs(node->n,
			 phnid, state,
			 twig_heap, twig_hkey,
			 phnidlst, statelst, nidlst,
			 free_key);
    }

    assert(FALSE);
}

/*
 * Log record.  Maintained by RCS.
 *
 * $Log$
 * Revision 1.5  2005/07/14  13:41:07  dhdfu
 * Bug 633610 (Carl Quillen): If the data set is very small, we may call
 * prune_leaves() with a leaf node.  Don't try to descend into subnodes
 * in that case!
 * 
 * Revision 1.4  2004/07/21 18:05:39  egouvea
 * Changed the license terms to make it the same as sphinx2 and sphinx3.
 *
 * Revision 1.3  2001/04/05 20:02:30  awb
 * *** empty log message ***
 *
 * Revision 1.2  2000/09/29 22:35:13  awb
 * *** empty log message ***
 *
 * Revision 1.1  2000/09/24 21:38:31  awb
 * *** empty log message ***
 *
 * Revision 1.6  97/07/23  10:49:21  eht
 * - Got rid of extraneous function
 * - Added get_node() call to get a tree node with a given id
 * 
 * Revision 1.5  97/07/17  15:25:41  eht
 * Fix bug where prune_subtrees() was giving incorrect count.
 * 
 * Revision 1.4  97/07/17  14:28:37  eht
 * Added prune_lowcnt() function to allow low occupancy states to be pruned.
 * 
 * Revision 1.3  97/07/16  12:17:25  eht
 * Fix bug in tied_state(); order of dfeat is l, b, r not b, l, r
 * 
 * Revision 1.2  97/07/16  11:36:22  eht
 * *** empty log message ***
 * 
 * Revision 1.1  97/07/07  10:53:10  eht
 * Initial revision
 * 
 *
 */
