/* -*- c-basic-offset: 4; indent-tabs-mode: nil -*- */
/* ====================================================================
 * Copyright (c) 2007 Carnegie Mellon University.  All rights
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
 * Author: David Huggins-Daines <dhuggins@cs.cmu.edu>
 */
/* Based on gstsphinxsink.c from gnome-voice-control:
 *
 * Copyright (C) 2007  Nickolay V. Shmyrev  <nshmyrev@yandex.ru>
 *
 * gstsphinxsink.c:
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */
/* Based on GStreamer plug-in template code, license follows: */
/*
 * GStreamer
 * Copyright 2005 Thomas Vander Stichele <thomas@apestaart.org>
 * Copyright 2005 Ronald S. Bultje <rbultje@ronald.bitfreak.net>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Alternatively, the contents of this file may be used under the
 * GNU Lesser General Public License Version 2.1 (the "LGPL"), in
 * which case the following provisions apply instead of the ones
 * mentioned above:
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/**
 * SECTION:element-plugin
 * @short_description: sink for speech recognition using PocketSphinx
 * <refsect2>
 * <title>Example launch line</title>
 * <para>
 * <programlisting>
 * gst-launch -v -m audiotestsrc ! pocketsphinx
 * </programlisting>
 * </para>
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gst/gst.h>
#include <gst/gstmarshal.h>
#include <sphinx_config.h>
#include <ad.h>
#include <string.h>

#include "gstpocketsphinx.h"

GST_DEBUG_CATEGORY_STATIC (gst_pocketsphinx_debug);
#define GST_CAT_DEFAULT gst_pocketsphinx_debug

/* Filter signals and args */
enum
{
    SIGNAL_INITIALIZATION,
    SIGNAL_AFTER_INITIALIZATION,
    SIGNAL_CALIBRATION,
    SIGNAL_LISTENING,
    SIGNAL_READY,
    SIGNAL_PARTIAL_RESULT,
    SIGNAL_RESULT,
    LAST_SIGNAL
};

enum
{
    PROP_0,
    PROP_HMM_DIR,
    PROP_LM_FILE,
    PROP_DICT_FILE,
    PROP_FSG_FILE,
    PROP_S2_FSG,
    PROP_FWDFLAT,
    PROP_BESTPATH
};

/* Default command line. (will go away soon and be constructed using properties) */
static char *default_argv[] = {
    "gst-pocketsphinx",
    "-hmm", POCKETSPHINX_PREFIX "/share/pocketsphinx/model/hmm/wsj1",
    "-lm", POCKETSPHINX_PREFIX "/share/pocketsphinx/model/lm/swb/swb.lm.DMP",
    "-dict", POCKETSPHINX_PREFIX "/share/pocketsphinx/model/lm/swb/swb.dic",
    "-samprate", "8000",
    "-nfft", "256",
    "-fwdflat", "no",
    "-bestpath", "no"
};
static const int default_argc = sizeof(default_argv)/sizeof(default_argv[0]);

static GstStaticPadTemplate sink_factory =
    GST_STATIC_PAD_TEMPLATE ("sink",
                             GST_PAD_SINK,
                             GST_PAD_ALWAYS,
                             GST_STATIC_CAPS("audio/x-raw-int, "
                                             "width = (int) 16, "
                                             "depth = (int) 16, "
                                             "endianness = (int) BYTE_ORDER, "
                                             "channels = (int) 1, "
                                             "rate = (int) 8000")
        );
	
static void gst_pocketsphinx_set_property (GObject * object, guint prop_id,
					   const GValue * value, GParamSpec * pspec);
static void gst_pocketsphinx_get_property (GObject * object, guint prop_id,
					   GValue * value, GParamSpec * pspec);
static gboolean gst_pocketsphinx_start(GstBaseSink * asink);
static gboolean gst_pocketsphinx_stop (GstBaseSink * asink);
static GstFlowReturn gst_pocketsphinx_render (GstBaseSink * sink, GstBuffer * buffer);
static void gst_pocketsphinx_do_init (GType type);

static guint gst_pocketsphinx_signals[LAST_SIGNAL];

GST_BOILERPLATE_FULL (GstPocketSphinx, gst_pocketsphinx, GstBaseSink,
		      GST_TYPE_BASE_SINK, gst_pocketsphinx_do_init);

static void
gst_pocketsphinx_do_init (GType type)
{
    GST_DEBUG_CATEGORY_INIT (gst_pocketsphinx_debug, "pocketsphinx",
                             0, "PocketSphinx plugin");
}


static void
gst_pocketsphinx_base_init (gpointer gclass)
{
    static GstElementDetails element_details = {
        "PocketSphinx",
        "Sink/Audio",
        "Perform speech recognition on a stream",
        "David Huggins-Daines <dhuggins@cs.cmu.edu>"
    };
    GstElementClass *element_class = GST_ELEMENT_CLASS (gclass);

    gst_element_class_add_pad_template (element_class,
                                        gst_static_pad_template_get (&sink_factory));
    gst_element_class_set_details (element_class, &element_details);
}

static void
string_disposal (gpointer key, gpointer value, gpointer user_data)
{
    g_free(value);
}

static void
gst_pocketsphinx_finalize (GObject * gobject)
{
    GstPocketSphinx *sphinxsink = GST_POCKETSPHINX (gobject);

    gst_adapter_clear (sphinxsink->adapter);
    g_object_unref (sphinxsink->adapter);

    g_hash_table_foreach (sphinxsink->arghash, string_disposal, NULL);
    g_hash_table_destroy (sphinxsink->arghash);

    GST_CALL_PARENT (G_OBJECT_CLASS, finalize, (gobject));
}

static void
gst_pocketsphinx_class_init (GstPocketSphinxClass * klass)
{
    GstBaseSinkClass *basesink_class;
    GObjectClass *gobject_class;

    gobject_class = (GObjectClass *) klass;
    basesink_class = (GstBaseSinkClass *) klass;

    gobject_class->set_property = gst_pocketsphinx_set_property;
    gobject_class->get_property = gst_pocketsphinx_get_property;
    gobject_class->finalize = GST_DEBUG_FUNCPTR (gst_pocketsphinx_finalize);
    basesink_class->start = GST_DEBUG_FUNCPTR (gst_pocketsphinx_start);
    basesink_class->stop = GST_DEBUG_FUNCPTR (gst_pocketsphinx_stop);
    basesink_class->render = GST_DEBUG_FUNCPTR (gst_pocketsphinx_render);

    /* TODO: We will bridge cmd_ln.h properties to GObject
     * properties here somehow eventually. */
    g_object_class_install_property
        (gobject_class, PROP_HMM_DIR,
         g_param_spec_string ("hmm", "HMM Directory",
                              "Directory containing acoustic model parameters",
                              POCKETSPHINX_PREFIX "/share/pocketsphinx/model/hmm/wsj1",
                              G_PARAM_READWRITE));
    g_object_class_install_property
        (gobject_class, PROP_LM_FILE,
         g_param_spec_string ("lm", "LM File",
                              "Language model file",
                              POCKETSPHINX_PREFIX "/share/pocketsphinx/model/lm/swb/swb.lm.DMP",
                              G_PARAM_READWRITE));
    g_object_class_install_property
        (gobject_class, PROP_FSG_FILE,
         g_param_spec_string ("fsg", "FSG File",
                              "Finite state grammar file",
                              NULL,
                              G_PARAM_READWRITE));
    g_object_class_install_property
        (gobject_class, PROP_S2_FSG,
         g_param_spec_pointer ("s2_fsg", "FSG Pointer",
                              "Finite state grammar pointer (s2_fsg_t *)",
                              G_PARAM_WRITABLE));
    g_object_class_install_property
        (gobject_class, PROP_DICT_FILE,
         g_param_spec_string ("dict", "Dictionary File",
                              "Dictionary File",
                              POCKETSPHINX_PREFIX "/share/pocketsphinx/model/lm/swb/swb.dic",
                              G_PARAM_READWRITE));

    g_object_class_install_property
        (gobject_class, PROP_FWDFLAT,
         g_param_spec_boolean ("fwdflat", "Flat Lexicon Search",
                              "Enable Flat Lexicon Search",
                               FALSE,
                               G_PARAM_READWRITE));
    g_object_class_install_property
        (gobject_class, PROP_BESTPATH,
         g_param_spec_boolean ("bestpath", "Graph Search",
                              "Enable Graph Search",
                               FALSE,
                               G_PARAM_READWRITE));


    gst_pocketsphinx_signals[SIGNAL_INITIALIZATION] =
        g_signal_new ("initialization", G_TYPE_FROM_CLASS (klass), G_SIGNAL_RUN_LAST,
                      G_STRUCT_OFFSET (GstPocketSphinxClass, initialization), NULL, NULL,
                      g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0, G_TYPE_NONE);

    gst_pocketsphinx_signals[SIGNAL_AFTER_INITIALIZATION] =
        g_signal_new ("after_initialization", G_TYPE_FROM_CLASS (klass), G_SIGNAL_RUN_LAST,
                      G_STRUCT_OFFSET (GstPocketSphinxClass, after_initialization), NULL, NULL,
                      g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0, G_TYPE_NONE);

    gst_pocketsphinx_signals[SIGNAL_CALIBRATION] =
        g_signal_new ("calibration", G_TYPE_FROM_CLASS (klass), G_SIGNAL_RUN_LAST,
                      G_STRUCT_OFFSET (GstPocketSphinxClass, calibration), NULL, NULL,
                      g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0, G_TYPE_NONE);

    gst_pocketsphinx_signals[SIGNAL_LISTENING] =
        g_signal_new ("listening", G_TYPE_FROM_CLASS (klass), G_SIGNAL_RUN_LAST,
                      G_STRUCT_OFFSET (GstPocketSphinxClass, listening), NULL, NULL,
                      g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0, G_TYPE_NONE);

    gst_pocketsphinx_signals[SIGNAL_READY] =
        g_signal_new ("ready", G_TYPE_FROM_CLASS (klass), G_SIGNAL_RUN_LAST,
                      G_STRUCT_OFFSET (GstPocketSphinxClass, calibration), NULL, NULL,
                      g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0, G_TYPE_NONE);

    gst_pocketsphinx_signals[SIGNAL_PARTIAL_RESULT] = 
        g_signal_new ("partial_result",
                      G_TYPE_FROM_CLASS (klass),
                      G_SIGNAL_RUN_LAST,
                      G_STRUCT_OFFSET (GstPocketSphinxClass, partial_result),
                      NULL, NULL,
                      gst_marshal_VOID__STRING,
                      G_TYPE_NONE,
                      1, G_TYPE_STRING
            );

    gst_pocketsphinx_signals[SIGNAL_RESULT] = 
        g_signal_new ("result",
                      G_TYPE_FROM_CLASS (klass),
                      G_SIGNAL_RUN_LAST,
                      G_STRUCT_OFFSET (GstPocketSphinxClass, result),
                      NULL, NULL,
                      gst_marshal_VOID__STRING,
                      G_TYPE_NONE,
                      1, G_TYPE_STRING
            );
}

static void
gst_pocketsphinx_set_string (GstPocketSphinx *sink,
                             const gchar *key, const GValue *value)
{
    /* NOTE: This is an undocumented feature of SphinxBase's cmd_ln.h.
     * However it will be officially supported in future releases. */
    anytype_t *val;
    gchar *str;

    val = cmd_ln_access(key);
    if (value != NULL)
        val->ptr = g_strdup(g_value_get_string(value));
    else
        val->ptr = NULL;
    if ((str = g_hash_table_lookup(sink->arghash, key)))
        g_free(str);
    g_hash_table_insert(sink->arghash, (gpointer)key, val->ptr);
}

static void
gst_pocketsphinx_set_boolean (GstPocketSphinx *sink,
                             const gchar *key, const GValue *value)
{
    /* NOTE: This is an undocumented feature of SphinxBase's cmd_ln.h.
     * However it will be officially supported in future releases. */
    anytype_t *val;
    val = cmd_ln_access(key);
    val->i_32 = g_value_get_boolean(value);
}

static void
gst_pocketsphinx_set_property (GObject * object, guint prop_id,
			       const GValue * value, GParamSpec * pspec)
{
    GstPocketSphinx *sink = GST_POCKETSPHINX(object);

    switch (prop_id) {
    case PROP_HMM_DIR:
        gst_pocketsphinx_set_string(sink, "-hmm", value);
        break;
    case PROP_LM_FILE:
        /* FSG and LM are mutually exclusive. */
        gst_pocketsphinx_set_string(sink, "-fsg", NULL);
        gst_pocketsphinx_set_string(sink, "-lm", value);
        /* Switch to this new LM if the decoder is active. */
        if (sink->ad.initialized) {
            lm_read(g_value_get_string(value),
                    g_value_get_string(value),
                    cmd_ln_float32("-lw"),
                    cmd_ln_float32("-uw"),
                    cmd_ln_float32("-wip"));
            uttproc_set_lm(g_value_get_string(value));
        }
        break;
    case PROP_DICT_FILE:
        gst_pocketsphinx_set_string(sink, "-dict", value);
        break;
    case PROP_FSG_FILE:
        /* FSG and LM are mutually exclusive */
        gst_pocketsphinx_set_string(sink, "-lm", NULL);
        gst_pocketsphinx_set_string(sink, "-fsg", value);
        /* Switch to this new FSG if the decoder is active. */
        if (sink->ad.initialized) {
            char *fsgname;

            fsgname = uttproc_load_fsgfile((char *)
                                           g_value_get_string(value));
            if (fsgname)
                uttproc_set_fsg(fsgname);
        }
        break;
    case PROP_S2_FSG:
    {
        s2_fsg_t *fsg = g_value_get_pointer(value);

        uttproc_del_fsg(fsg->name);
        uttproc_load_fsg(g_value_get_pointer(value),
                         cmd_ln_boolean("-fsgusealtpron"),
                         cmd_ln_boolean("-fsgusefiller"),
                         cmd_ln_float32("-silpen"),
                         cmd_ln_float32("-fillpen"),
                         cmd_ln_float32("-lw"));
        uttproc_set_fsg(fsg->name);
        break;
    }
    case PROP_FWDFLAT:
        gst_pocketsphinx_set_boolean(sink, "-fwdflat", value);
        break;
    case PROP_BESTPATH:
        gst_pocketsphinx_set_boolean(sink, "-bestpath", value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        return;
    }
}

static void
gst_pocketsphinx_get_property (GObject * object, guint prop_id,
			       GValue * value, GParamSpec * pspec)
{
    switch (prop_id) {
    case PROP_HMM_DIR:
        g_value_set_string(value, cmd_ln_str("-hmm"));
        break;
    case PROP_LM_FILE:
        g_value_set_string(value, cmd_ln_str("-lm"));
        break;
    case PROP_DICT_FILE:
        g_value_set_string(value, cmd_ln_str("-dict"));
        break;
    case PROP_FSG_FILE:
        g_value_set_string(value, cmd_ln_str("-fsg"));
        break;
    case PROP_FWDFLAT:
        g_value_set_boolean(value, cmd_ln_boolean("-fwdflat"));
        break;
    case PROP_BESTPATH:
        g_value_set_boolean(value, cmd_ln_boolean("-bestpath"));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void
gst_pocketsphinx_init (GstPocketSphinx * rec,
		       GstPocketSphinxClass * gclass)
{
    /* Create the hash table to store argument strings. */
    rec->arghash = g_hash_table_new(g_str_hash, g_str_equal);

    /* Parse default command-line options. */
    cmd_ln_parse(fbs_get_args(), default_argc, default_argv, FALSE);

    /* Create an adapter object to "pull" data from the pipeline */
    GST_BASE_SINK(rec)->sync = FALSE;
    rec->adapter = gst_adapter_new ();
}

#define REQUIRED_FRAME_SAMPLES 1024
#define REQUIRED_FRAME_BYTES  REQUIRED_FRAME_SAMPLES * 2

int32 
gst_pocketsphinx_ad_read(ad_rec_t *ad, int16 *buf, int32 max)
{
    GstPocketSphinx *sphinxsink = GST_POCKETSPHINX (((GstSphinxSinkAd *)ad)->self);

    memcpy ((void *)buf,
            gst_adapter_peek (sphinxsink->adapter, REQUIRED_FRAME_BYTES),
            REQUIRED_FRAME_BYTES);
  
    return REQUIRED_FRAME_SAMPLES;
}

static gboolean
gst_pocketsphinx_start (GstBaseSink * asink)
{
    GstPocketSphinx *sphinxsink = GST_POCKETSPHINX (asink);
    GstSphinxSinkAd *ad = &sphinxsink->ad;

    ad->self = sphinxsink;
    ad->sps = 16000;
    ad->self = sphinxsink;
    ad->bps = sizeof(int16);
    ad->calibrated = FALSE;
    ad->calibrate_started = FALSE;
  
    sphinxsink->cont = cont_ad_init ((ad_rec_t *)ad, gst_pocketsphinx_ad_read);
  
    return TRUE;
}

static gboolean
gst_pocketsphinx_stop (GstBaseSink * asink)
{
    GstPocketSphinx *sphinxsink = GST_POCKETSPHINX (asink);
  
    cont_ad_close (sphinxsink->cont);
  
    return TRUE;
}

static void gst_pocketsphinx_calibrate_chunk (GstPocketSphinx *sphinxsink)
{
    int result;

    if (!sphinxsink->ad.calibrate_started) {
        g_signal_emit (sphinxsink,
                       gst_pocketsphinx_signals[SIGNAL_CALIBRATION], 0, NULL);
        sphinxsink->ad.calibrate_started = TRUE;
    }

    result = cont_ad_calib_loop (sphinxsink->cont, 
                                 (int16 *)gst_adapter_peek(sphinxsink->adapter,
                                                           REQUIRED_FRAME_BYTES),
                                 REQUIRED_FRAME_SAMPLES);
	
    if (result == 0) {
        sphinxsink->ad.calibrated = TRUE;
        sphinxsink->ad.listening = 0;
    	    
        g_signal_emit (sphinxsink,
                       gst_pocketsphinx_signals[SIGNAL_READY], 0, NULL);
    }
}

static void gst_pocketsphinx_process_chunk (GstPocketSphinx *sphinxsink)
{
    int32 k;
    int16 adbuf[REQUIRED_FRAME_SAMPLES];
	
    k = cont_ad_read (sphinxsink->cont, adbuf, REQUIRED_FRAME_SAMPLES);
	
    if (k == 0 && sphinxsink->last_ts == 0) {
        return;
    } else if (k == 0 && sphinxsink->cont->read_ts - sphinxsink->last_ts > 
               DEFAULT_SAMPLES_PER_SEC) {
        int32 fr;
        char *hyp = NULL;
  
        uttproc_end_utt ();
        if (uttproc_result (&fr, &hyp, 1) < 0) {
            g_warning ("uttproc_result failed");
        } else {
            if (hyp != NULL)
                g_signal_emit (sphinxsink,
                               gst_pocketsphinx_signals[SIGNAL_RESULT], 
                               0, hyp);
        }

        sphinxsink->last_ts = 0;
        sphinxsink->ad.listening = 0;
        g_signal_emit (sphinxsink,
                       gst_pocketsphinx_signals[SIGNAL_READY], 0, NULL);

    } else if (k != 0) {
        if (sphinxsink->ad.listening == 0) {
            uttproc_begin_utt (NULL);
            sphinxsink->ad.listening = 1;
            g_signal_emit (sphinxsink,
                           gst_pocketsphinx_signals[SIGNAL_LISTENING], 0, NULL);
        }
	
        uttproc_rawdata (adbuf, k, 1);
        sphinxsink->last_ts = sphinxsink->cont->read_ts;
    }
}

static GstFlowReturn gst_pocketsphinx_render (GstBaseSink * asink, GstBuffer * buffer)
{
    GstPocketSphinx *sphinxsink = GST_POCKETSPHINX (asink);

    if (!sphinxsink->ad.initialized) {
        g_signal_emit (sphinxsink,
                       gst_pocketsphinx_signals[SIGNAL_INITIALIZATION], 0, NULL);
        /* We initialized the cmd_ln module earlier. */
        fbs_init(0, NULL);
        g_signal_emit (sphinxsink,
                       gst_pocketsphinx_signals[SIGNAL_AFTER_INITIALIZATION], 0, NULL);
        sphinxsink->ad.initialized = TRUE;
    }

    gst_buffer_ref (buffer);
    gst_adapter_push (sphinxsink->adapter, buffer);
  
    while (gst_adapter_available (sphinxsink->adapter) >= REQUIRED_FRAME_BYTES) {
	if (sphinxsink->ad.calibrated) {
            gst_pocketsphinx_process_chunk (sphinxsink);
    	} else {
            gst_pocketsphinx_calibrate_chunk (sphinxsink);
    	}
        gst_adapter_flush (sphinxsink->adapter, REQUIRED_FRAME_BYTES);
    }

    return GST_FLOW_OK;
}

static gboolean
plugin_init (GstPlugin * plugin)
{
    return gst_element_register (plugin, "pocketsphinx",
                                 GST_RANK_NONE, GST_TYPE_POCKETSPHINX);
}

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
		   GST_VERSION_MINOR,
		   "pocketsphinx",
		   "PocketSphinx plugin",
		   plugin_init, VERSION,
		   "LGPL", "PocketSphinx", "http://pocketsphinx.org/")
