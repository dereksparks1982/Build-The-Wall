# Build the Wall - Offline Archive

A preserved, offline Linux package of the **Build the Wall** browser game that was briefly available through the White House Arcade.

## Original archival source

Original page:

`https://www.whitehouse.gov/arcade/build-the-wall/`

Exact Wayback capture used as the recovery source:

`https://web.archive.org/web/20260904000052/https://www.whitehouse.gov/arcade/build-the-wall/`

Wayback capture timestamp: **September 4, 2026 at 00:00:52 UTC**.

For broader capture history:

`https://web.archive.org/web/*/https://www.whitehouse.gov/arcade/build-the-wall/`

## What this repository is

This repository preserves the recovered browser game as a self-contained offline Linux program. The preserved game is served only on the local loopback interface and opened in a supported browser.

The work in this repository is the offline-preservation adaptation: recovering the archived page, repairing viewport behavior, embedding the recovered HTML in a native launcher, and packaging the result so it can run without depending on the original website.

## Recovery and offline conversion

The archived White House page was recovered from the exact Wayback Machine capture linked above. The game page was then converted into a local package in several stages:

1. The recovered browser content was saved as a local `index.html`.
2. A viewport-fit repair was added so the complete arcade cabinet scales to the available browser viewport while preserving aspect ratio and controls at normal browser zoom.
3. The resulting HTML was converted byte-for-byte into `game_html.inc` and embedded directly into the native launcher.
4. `main.c` implements a small HTTP server bound only to `127.0.0.1` on an automatically selected free port.
5. The launcher starts a supported installed browser and opens the locally served game.
6. The resulting `Build_The_Wall` executable is a ready-to-run x86-64 Linux ELF program.

The launcher tries these browsers in order when available:

`chromium`, `chromium-browser`, `google-chrome`, `google-chrome-stable`, `brave-browser`, `microsoft-edge`, then `firefox`.

## Technical behavior

The executable does not expose a network service to other machines. It binds to:

`127.0.0.1:<ephemeral-port>`

The embedded game HTML is returned directly from memory with `Cache-Control: no-store`. The game therefore does not require the original White House page to remain online.

## Running

```bash
chmod +x Build_The_Wall
./Build_The_Wall
```

The game should open automatically in a supported browser.

## Reproducibility

The preserved executable was verified against a rebuild using:

```bash
gcc -O2 -s -o Build_The_Wall main.c
```

with GCC 14.2.0 (`Debian 14.2.0-19`). That rebuild reproduced the preserved executable byte-for-byte.

## Repository files

| File | Purpose |
| --- | --- |
| `Build_The_Wall` | Ready-to-run Linux x86-64 executable |
| `Build_The_Wall.desktop` | Linux desktop launcher entry |
| `main.c` | Native localhost launcher/server source |
| `game_html.inc` | Embedded recovered game HTML |
| `index.html` | Recovered and viewport-repaired browser content |
| `PACKAGE_README.txt` | Original package README |
| `SHA256SUMS` | Integrity hashes for the preserved package |
| `THIRD_PARTY_NOTICE.md` | Rights, attribution, and takedown notice |

## Ownership and license scope

This repository does **not** claim ownership of the underlying archived game, White House branding, names, likenesses, trademarks, artwork, or other third-party material.

The MIT license applies only to original preservation/adaptation work contributed here, including the native localhost launcher/server, viewport-fit repair, packaging metadata, and original documentation, to the extent those portions are independently copyrightable by the contributor.

See `THIRD_PARTY_NOTICE.md` for additional detail.

## Preservation purpose

This project exists to keep a short-lived browser game accessible after its original web distribution disappeared. The exact archival source is documented above so the provenance of the preserved copy can be independently checked.
