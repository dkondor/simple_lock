/*
 * simple_lock.c -- simple wrapper for ext-session-lock without creating actual lock surfaces
 * 
 * Copyright 2024 Daniel Kondor <kondor.dani@gmail.com>
 * 
 * This is free and unencumbered software released into the public
 * domain. See the file LICENSE for details.
 * 
 */


#include <simple_lock.h>
#include <stdio.h>
#include <string.h>
#include <wayland-client.h>
#include <ext-session-lock-v1-client-protocol.h>

struct wl_display *display = NULL;
static struct ext_session_lock_manager_v1 *manager = NULL;
static struct ext_session_lock_v1 *current_lock = NULL;
static bool is_locked = false;

static void _add (void*, struct wl_registry *registry,
		uint32_t name, const char *interface, uint32_t) {
	if (strcmp (interface, ext_session_lock_manager_v1_interface.name) == 0)
		manager = (struct ext_session_lock_manager_v1*)wl_registry_bind (registry, name, &ext_session_lock_manager_v1_interface, 1u);
}

static void _remove (void*, struct wl_registry*, uint32_t) { }

static struct wl_registry_listener listener = { &_add, &_remove };


static void _locked (void*, struct ext_session_lock_v1*)
{
	is_locked = true;
	printf ("screen locked\n");
}

static void _finished (void*, struct ext_session_lock_v1*)
{
	printf ("screen lock finished\n");
	simple_lock_unlock ();
}

static struct ext_session_lock_v1_listener lock_listener = { &_locked, &_finished };



/* Try to initialize the session lock interface. */
bool simple_lock_init ()
{
	display = wl_display_connect (NULL);
	if (!display) return false;
	
	struct wl_registry* registry = wl_display_get_registry (display);
	if (!registry) return false;

	wl_registry_add_listener (registry, &listener, NULL);
	wl_display_dispatch (display);
	wl_display_roundtrip (display);
	if (!manager) return false;
	return true;
}

/* Get the wayland display used. */
struct wl_display *simple_lock_get_display (void)
{
	return display;
}

/* Try to lock the screen. */
void simple_lock_lock ()
{
	if (current_lock) return;
	if (!manager) return;
	printf ("trying to lock the screen\n");
	is_locked = false;
	current_lock = ext_session_lock_manager_v1_lock (manager);
	ext_session_lock_v1_add_listener (current_lock, &lock_listener, NULL);
}

/* Unlock the screen. */
void simple_lock_unlock ()
{
	if (!current_lock) return;
	printf ("trying to unlock the screen\n");
	if (is_locked) ext_session_lock_v1_unlock_and_destroy (current_lock);
	else ext_session_lock_v1_destroy (current_lock);
	current_lock = NULL;
}

void simple_lock_fini ()
{
	simple_lock_unlock ();
	if (display)
	{
		wl_display_disconnect (display);
		display = NULL;
	}
	manager = NULL;
}

