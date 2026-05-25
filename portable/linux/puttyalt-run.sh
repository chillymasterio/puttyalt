#!/bin/bash
DIR="$(cd "$(dirname "$0")" && pwd)"
chmod +x "$DIR/puttyalt" 2>/dev/null
exec "$DIR/puttyalt" "$@"
