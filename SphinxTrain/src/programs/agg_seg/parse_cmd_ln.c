/* ====================================================================
 * Copyright (c) 1994-2000 Carnegie Mellon University.  All rights 
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
 * File: parse_cmd_ln.c
 * 
 * Description: 
 * 
 * Author: 
 * 
 *********************************************************************/

#include "parse_cmd_ln.h"

#include <s3/cmd_ln.h>
#include <s3/err.h>

#include <stdio.h>
#include <stdlib.h>

int
parse_cmd_ln(int argc, char *argv[])
{
  uint32      isHelp;
  uint32      isExample;

    const char helpstr[] =  
"Description: \n\
\n\
agg_seg accumulate feature vectors and use the accumulated values to quantize \n\
the vector space.  This functionality is useful in semi-continuous training \n\
initialization. Feature vectors are skipped according to the option -stride. \n\
\n\
Some options are specific to Sphinx-2.\n\
Please type -example yes to get an example argument list.";

    const char examplestr[]=
"Example: \n\
\n\
agg_seg -segdmpdirs segdmpdir -segdmpfn dumpfile -segtype all -ctlfn \n\
ctl -cepdir cepdir -cepext .mfc -ceplen 13 -stride 10 ";


    static arg_def_t defn[] = {
	{ "-help",
	  CMD_LN_BOOLEAN,
	  CMD_LN_NO_VALIDATION,
	  "no",
	  "Shows the usage of the tool"},

	{ "-example",
	  CMD_LN_BOOLEAN,
	  CMD_LN_NO_VALIDATION,
	  "no",
	  "Shows example of how to use the tool"},

	{ "-segdmpdirs",
	  CMD_LN_STRING_LIST,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Segment dump directories" },
	{ "-segdmpfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Segment dump file" },
	{ "-segidxfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Segment index into the dump file." },
	{ "-segtype",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  "st",
	  "Type of segments to dump.  {all,st,phn}" },

	{ "-cntfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Per id count file" },

	{ "-ddcodeext",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  "xcode",
	  "Extension of the VQ 2nd difference cepstrum files"},

	{ "-lsnfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Lexical transcript file (contains all utts in ctl file)" },
	{ "-sentdir",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Root directory of sent (lexical transcript) files" },
	{ "-sentext",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Extension of sent (lexical transcript) files" },
	{ "-ctlfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "The control file name (enumerates utts in corpus)" },
	{ "-mllrctlfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Lists the MLLR transforms for each utterance" },
	{ "-mllrdir",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Directory for MLLR matrices" },
	{ "-nskip",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  "0",
	  "The number of utterances to skip in the control file"},
	{ "-runlen",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  "-1",
	  "The number of utterances to process after skipping"},
	{ "-moddeffn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Model definition file containing all the triphones in the corpus.  State/transition matrix definitions are ignored." },
	{ "-ts2cbfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Tied state to codebook mapping file (may be '.semi.' or '.cont.')"},
	{ "-cb2mllrfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  ".1cls.",
	  "codebook to MLLR class mapping file (may be '.1cls.')"},
	{ "-dictfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Lexicon containing all the words in the lexical transcripts."},
	{ "-fdictfn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Lexicon containing all the filler words in the lexical transcripts."},

	{ "-segdir",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Root directory of the state segmentation files"},
	{ "-segext",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  "v8_seg",
	  "Extension of the state segmentation files"},

	{ "-ccodedir",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Root directory of the VQ cepstrum files"},
	{ "-ccodeext",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  "ccode",
	  "Extension of the VQ cepstrum files"},

	{ "-dcodedir",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Root directory of the VQ difference cepstrum files"},
	{ "-dcodeext",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  "d2code",
	  "Extension of the VQ cepstrum files"},

	{ "-pcodedir",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Root directory of the VQ power files"},
	{ "-pcodeext",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  "p3code",
	  "Extension of the VQ power files"},

	{ "-ddcodedir",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Root directory of the VQ 2nd difference cepstrum files"},
	{ "-ddcodeext",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  "xcode",
	  "Extension of the VQ 2nd difference cepstrum files"},

	{ "-cepdir",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "Root directory of the cepstrum files"},
	{ "-cepext",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  FEAT_DEFAULT_FEATURE_EXTENSION,
	  "Extension of the cepstrum files"},
	{ "-ceplen",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  FEAT_DEFAULT_CEP_LENGTH,
	  "# of coefficients per cepstrum frame"},
	{ "-cepwin",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  "0",
	  "sliding window of features to concatenate (for -feat 1s_c ONLY)"},

	{ "-agc",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  "max",
	  "The type of automatic gain control to do {max|emax}"},
	{ "-cmn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  "current",
	  "The do cepstral mean normalization based on {current|prior} utterance(s)"},
	{ "-varnorm",
	  CMD_LN_BOOLEAN,
	  CMD_LN_NO_VALIDATION,
	  "no",
	  "Normalize utterance by its variance"},
	{ "-silcomp",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  "none",
	  "Do silence compression based on {current|prior} utterance"},
	{ "-feat",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  FEAT_DEFAULT_FEATURE_TYPE,
	  "Feature set to compute"},
	{ "-svspec",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  NULL,
	  "Split single stream features into subvectors according to this specification."},
	{ "-ldafn",
	  CMD_LN_STRING,
	  CMD_LN_NO_VALIDATION,
	  CMD_LN_NO_DEFAULT,
	  "File containing an LDA transformation matrix."},
	{ "-ldadim",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  "29",
	  "# of output dimensions for LDA"},

	{ "-cachesz",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  "200",
	  "Feature cache size in Mb"},

	{ "-stride",
	  CMD_LN_INT32,
	  CMD_LN_NO_VALIDATION,
	  "1",
	  "Take every -stride'th frame when producing dmp" },

	{ NULL, CMD_LN_UNDEF, CMD_LN_NO_VALIDATION, CMD_LN_NO_DEFAULT, NULL }
    };

    cmd_ln_define(defn);

    if (argc == 1) {
	cmd_ln_print_definitions();
	exit(1);
    }

    cmd_ln_parse(argc, argv);

    if (cmd_ln_validate() == FALSE) {
	E_FATAL("Unable to validate command line arguments\n");
    }

    isHelp    = *(uint32 *) cmd_ln_access("-help");
    isExample    = *(uint32 *) cmd_ln_access("-example");


    if(isHelp){
      printf("%s\n\n",helpstr);
    }

    if(isExample){
      printf("%s\n\n",examplestr);
    }

    if(isHelp || isExample){
      E_INFO("User asked for help or example.\n");
      exit(1);
    }
    if(!isHelp && !isExample){
      cmd_ln_print_configuration();
    }


    return 0;
}

/*
 * Log record.  Maintained by RCS.
 *
 * $Log$
 * Revision 1.7  2004/11/29  01:43:44  egouvea
 * Replaced handling of help or example so that user gets an INFO message instead of a scarier FATAL_ERROR
 * 
 * Revision 1.6  2004/11/29 00:49:19  egouvea
 * Added missing include files to prevent warnings about printf not being defined
 *
 * Revision 1.5  2004/08/08 01:20:37  arthchan2003
 * agg_seg help and example strings
 *
 * Revision 1.4  2004/07/21 18:30:32  egouvea
 * Changed the license terms to make it the same as sphinx2 and sphinx3.
 *
 * Revision 1.3  2001/04/05 20:02:31  awb
 * *** empty log message ***
 *
 * Revision 1.2  2000/09/29 22:35:13  awb
 * *** empty log message ***
 *
 * Revision 1.1  2000/09/24 21:38:31  awb
 * *** empty log message ***
 *
 * Revision 1.4  97/07/16  11:36:22  eht
 * *** empty log message ***
 * 
 * Revision 1.3  96/03/25  15:45:23  eht
 * Development version
 * 
 * Revision 1.2  1996/03/04  16:01:06  eht
 * *** empty log message ***
 *
 * Revision 1.1  1995/12/15  18:37:07  eht
 * Initial revision
 *
 *
 */
