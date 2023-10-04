# Git safety mirror

A simple utility to keep track of the history of repositories which sometimes rebase and force-push.

The problem with force-pushing is that git references can disappear, and projects depending on them stop working.
Such projects can depend on a safety mirror instead, and their builds will keep working.

## Usage

* Setup: Replace the example in `source.json` with the repositories you want to track
* Usage: Run `safety-mirror.py` and your repository will have the branches you depend on in its history
