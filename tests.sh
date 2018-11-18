#!/usr/bin/env bash

source $HOME/dotfiles/scripts/assert.sh/assert.sh

FILE="${BASH_SOURCE[0]}"
FILE="$(realpath $FILE)"
if [[ -L "$FILE" ]]; then
    FILE="$(readlink $FILE)"
fi
echo "$FILE"
DIR=$(dirname "$FILE")
DIR=$(realpath "${DIR}")    # resolve its full path if need be

# test a binary template
cd "$DIR/src/build"
conan install ../..
conan build ../..
rm -rf cligod
bin/conanmoban bin cligod
cd cligod/src/build
conan install ../..
conan build ../..
echo "cligod: " $(bin/cligod)
assert_eq $"hello" $(bin/cligod)
if [[ $? != 0 ]]; then
(>&2 echo "cligod should output 'hello'")
fi


# # test a header-only library template
# cd "$DIR/src/build"
# conan install ../..
# conan build ../..
# rm -rf cligod
# bin/conanmoban bin cligod
# cd cligod/src/build
# conan install ../..
# conan build ../..
# echo "cligod: " $(bin/cligod)
# assert_eq $"hello" $(bin/cligod)
# if [[ $? != 0 ]]; then
# (>&2 echo "cligod should output 'hello'")
# fi
