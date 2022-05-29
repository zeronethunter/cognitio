#! /bin/sh

GIT_HOME=$(pwd)
while [ ! -d ".git" ] && [ "$GIT_HOME" != "/" ]; do
  GIT_HOME=$(dirname "$GIT_HOME")
done

if [ "$GIT_HOME" = "/" ]; then
  echo "Unable to find git repo. Cannot create pin files for Cognitio repo"
  exit 1
fi

EXAMPLES_DIR=examples
if [ -d $EXAMPLES_DIR ]; then
  i=1
  while [ -d "$EXAMPLES_DIR/repo$i" ]; do
    touch $EXAMPLES_DIR/repo$i/pins
    i=$((i+1))
  done
fi
