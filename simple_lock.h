/*
 * simple_lock.h -- simple wrapper for ext-session-lock without creating actual lock surfaces
 * 
 * Copyright 2024 Daniel Kondor <kondor.dani@gmail.com>
 * 
 * This is free and unencumbered software released into the public
 * domain. See the file LICENSE for details.
 * 
 */


#ifndef SIMPLE_LOCK_H
#define SIMPLE_LOCK_H

#include <stdbool.h>
#include <wayland-client.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Try to initialize the session lock interface. */
bool simple_lock_init (void);

/* Get the wayland display used. */
struct wl_display *simple_lock_get_display (void);

/* Try to lock the screen. */
void simple_lock_lock (void);

/* Unlock the screen. */
void simple_lock_unlock (void);

/* free all resources */
void simple_lock_fini (void);

#ifdef __cplusplus
}
#endif

#endif

