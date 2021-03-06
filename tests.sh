#!/usr/bin/env bash


FILE="${BASH_SOURCE[0]}"
FILE="$(realpath $FILE)"
if [[ -L "$FILE" ]]; then
    FILE="$(readlink $FILE)"
fi
echo "$FILE"
DIR=$(dirname "$FILE")
DIR=$(realpath "${DIR}")    # resolve its full path if need be
source "$DIR/assert.sh"

# test a binary template
cd "$DIR/build"
conan install --build missing ..
conan build ..
rm -rf cligod
bin/conanmoban bin cligod
cd cligod/build
conan install --build missing ..
conan build ..
echo "cligod: " $(bin/cligod)
assert_eq $"hello" $(bin/cligod)
if [[ $? != 0 ]]; then
(>&2 echo "cligod should output 'hello'")
fi


# test a header-only library template
cd "$DIR/build"
rm -rf headergod
bin/conanmoban lib  headergod --header_only
cd headergod/build
conan install --build missing ..
conan build ..
assert_eq "3" $(conan create .. jzien/dev | tail -n1)
if [[ $? != 0 ]]; then
(>&2 echo "headergod should output '3'")
fi

# test a library template
cd "$DIR/build"
rm -rf libgod
bin/conanmoban lib  libgod
cd libgod/build
conan install --build missing ..
conan build ..
assert_eq "3" $(conan create .. jzien/dev | tail -n1)
if [[ $? != 0 ]]; then
(>&2 echo "libgod test should output '3'")
fi
