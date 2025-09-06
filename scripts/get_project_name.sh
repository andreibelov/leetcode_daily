#!/usr/bin/env bash
set -euo pipefail

DEFAULT_NAME="${DEFAULT_NAME:- }"
MAXLEN="${MAXLEN:-45}"

default="$DEFAULT_NAME"

while :; do
    echo "Enter name of the new project to be created:" >&2;
    read -e -r -i "$default" -p "> " raw || { echo "aborted" >&2; exit 2; }

    # trim leading/trailing whitespace
    raw="$(printf "%s" "$raw" | sed -e 's/^[[:space:]]*//' -e 's/[[:space:]]*$//')"

    if [ -z "$raw" ]; then
        echo "Please enter a non-empty name." >&2
        default=""
        continue
    fi

    # Normalize raw name: tolower, non-alnum -> '_', collapse, trim, cut to MAXLEN
    name="$(printf "%s\n" "$raw" | awk -v maxlen="$MAXLEN" '
        {
            s = tolower($0);
            gsub(/[^a-z0-9]+/, "_", s);
            gsub(/^_+|_+$$/, "", s);
            gsub(/_+/, "_", s);
            if (length(s) > maxlen) s = substr(s, 1, maxlen);
            print s;
        }
    ')"

    if [ -z "$name" ]; then
        echo -e "Resulting name is empty after sanitization, try again.\n" >&2
        default="$raw"
        continue
    fi

    if [ -e "$name" ]; then
        echo "Path '$name' already exists, pick another." >&2
        default="$raw"
        continue
    fi
    printf "%s\n" "$name"
    break
done
