/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2009 Richard Hughes <richard@hughsie.com>
 *
 * Licensed under the GNU General Public License Version 2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/**
 * SECTION:pk-files
 * @short_description: Files object
 *
 * This GObject represents a files from a transaction.
 * These objects represent single items of data from the transaction, and are
 * often present in lists (#PkResults) or just refcounted in client programs.
 */

#include "config.h"

#include <glib-object.h>

#include <packagekit-glib2/pk-files.h>

#include "egg-debug.h"

static void     pk_files_finalize	(GObject     *object);

#define PK_FILES_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), PK_TYPE_FILES, PkFilesPrivate))

/**
 * PkFilesPrivate:
 *
 * Private #PkFiles data
 **/
struct _PkFilesPrivate
{
	gchar				*package_id;
	gchar				**files;
};

enum {
	PROP_0,
	PROP_PACKAGE_ID,
	PROP_FILES,
	PROP_LAST
};

G_DEFINE_TYPE (PkFiles, pk_files, PK_TYPE_SOURCE)

/**
 * pk_files_get_property:
 **/
static void
pk_files_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	PkFiles *files = PK_FILES (object);
	PkFilesPrivate *priv = files->priv;

	switch (prop_id) {
	case PROP_PACKAGE_ID:
		g_value_set_string (value, priv->package_id);
		break;
	case PROP_FILES:
		g_value_set_boxed (value, priv->files);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

/**
 * pk_files_set_property:
 **/
static void
pk_files_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	PkFiles *files = PK_FILES (object);
	PkFilesPrivate *priv = files->priv;

	switch (prop_id) {
	case PROP_PACKAGE_ID:
		g_free (priv->package_id);
		priv->package_id = g_strdup (g_value_get_string (value));
		break;
	case PROP_FILES:
		g_strfreev (priv->files);
		priv->files = g_strdupv (g_value_get_boxed (value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

/**
 * pk_files_class_init:
 **/
static void
pk_files_class_init (PkFilesClass *klass)
{
	GParamSpec *pspec;
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	object_class->finalize = pk_files_finalize;
	object_class->get_property = pk_files_get_property;
	object_class->set_property = pk_files_set_property;

	/**
	 * PkFiles:package-id:
	 */
	pspec = g_param_spec_string ("package-id", NULL, NULL,
				     NULL,
				     G_PARAM_READWRITE);
	g_object_class_install_property (object_class, PROP_PACKAGE_ID, pspec);

	/**
	 * PkFiles:files:
	 */
	pspec = g_param_spec_boxed ("files", NULL, NULL,
				    G_TYPE_STRV,
				    G_PARAM_READWRITE);
	g_object_class_install_property (object_class, PROP_FILES, pspec);

	g_type_class_add_private (klass, sizeof (PkFilesPrivate));
}

/**
 * pk_files_init:
 **/
static void
pk_files_init (PkFiles *files)
{
	files->priv = PK_FILES_GET_PRIVATE (files);
	files->priv->package_id = NULL;
	files->priv->files = NULL;
}

/**
 * pk_files_finalize:
 **/
static void
pk_files_finalize (GObject *object)
{
	PkFiles *files = PK_FILES (object);
	PkFilesPrivate *priv = files->priv;

	g_free (priv->package_id);
	g_strfreev (priv->files);

	G_OBJECT_CLASS (pk_files_parent_class)->finalize (object);
}

/**
 * pk_files_new:
 *
 * Return value: a new PkFiles object.
 **/
PkFiles *
pk_files_new (void)
{
	PkFiles *files;
	files = g_object_new (PK_TYPE_FILES, NULL);
	return PK_FILES (files);
}

