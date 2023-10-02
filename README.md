# Git safety mirror

A utility to keep track of the history of repositories which sometimes rebase and force-push.

The problem with force-pushing is that git references can disappear, and projects depending on them stop working.
Such projects can depend on a safety mirror instead, and their builds will keep working.
