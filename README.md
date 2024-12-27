## simple_lock

A test client for the [ext-session-lock-v1](https://wayland.app/protocols/ext-session-lock-v1) Wayland protocol.

**Warning**: this program is only intended for demonstration purposes! Do not use as a screenlocker.

This is a simple client that locks the screen using the `ext-session-lock-v1` protocol without creating a lock surface (or any form of UI at all).
The purpose is to test whether compositors actually lock the screen in this case.

There is no UI for unlocking the screen, but it will be unlocked if a SIGINT or SIGTERM signal is received. It is recommended to only run this
code in a nested compositor to avoid locking yourself out of a session.

Compile with meson:

```
meson setup -Dbuildtype=debug build
ninja -C build
# lock the screen for 5 seconds
build/simple_lock & sleep 5; kill %1;
```
