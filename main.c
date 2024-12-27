/*
 * main.c -- simple example client for the ext-session-lock-v1 protocol
 *   that does not create a lock surface
 * 
 * Copyright 2024 Daniel Kondor <kondor.dani@gmail.com>
 * 
 * This is free and unencumbered software released into the public
 * domain. See the file LICENSE for details.
 * 
 */

#define _GNU_SOURCE

#include <simple_lock.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

static bool quit_request = false;
static int pfd[2];

static void sigterm (int)
{
	char tmp = 0;
	quit_request = true;
	write (pfd[1], &tmp, 1);
}

int main(int argc, char **argv)
{
	if (!simple_lock_init ())
	{
		fprintf (stderr, "Cannot init!\n");
		return 1;
	}
	
	signal (SIGTERM, sigterm);
	signal (SIGINT, sigterm);
	
	pipe2 (pfd, O_CLOEXEC);
	
	struct wl_display *dsp = simple_lock_get_display ();
	
	simple_lock_lock ();
	wl_display_flush (dsp);
		
	struct pollfd fds[2];
	fds[0].fd = wl_display_get_fd (dsp);
	fds[0].events = POLLIN | POLLHUP;
	fds[1].fd = pfd[0];
	fds[1].events = POLLIN;
	
	// ensure we don't have any unprocessed events
	while (wl_display_prepare_read (dsp) != 0) wl_display_dispatch_pending (dsp);
	
	while (true)
	{
		poll (fds, 1, -1);
		wl_display_read_events (dsp);
		wl_display_dispatch_pending (dsp);
		if (quit_request) break;
		wl_display_prepare_read (dsp);
	}

	simple_lock_unlock ();
	wl_display_roundtrip (dsp);
	simple_lock_fini ();
	close (pfd[0]);
	close (pfd[1]);
	
	return 0;
}

