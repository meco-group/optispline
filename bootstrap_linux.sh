sudo apt-get update
sudo apt-get install -y openssh-client git wget python2.7-dev cmake g++-4.7


if [ -f ~/.ssh/id_rsa.pub ];
then
  echo "ssh key found"
else
  ssh-keygen -q -t rsa -N "" -f ~/.ssh/id_rsa
fi

echo "======================================================"
echo "User action needed"
echo "  Go to https://gitlab.mech.kuleuven.be/profile"
echo "  click on the 'SSH Keys' tab"
echo "  Paste the following in the 'Key' field: "
echo ""
cat ~/.ssh/id_rsa.pub
echo ""
echo "  Give it some Title and click 'Add key'"
echo "  Press enter after doing this"
read ANSWER

cd ~ && wget -O - "https://www.dropbox.com/download?plat=lnx.x86_64" | tar xzf -

~/.dropbox-dist/dropboxd &

echo "======================================================"
echo "Press enter after visiting the Dropbox link"

read ANSWER

ssh-keyscan gitlab.mech.kuleuven.be >> ~/.ssh/known_hosts

git clone git@gitlab.mech.kuleuven.be:meco-software/cpp_splines.git

cd cpp_splines

mkdir build-matlab
mkdir build-python

cat <<EOF > build-python/run.sh
rm CMakeCache.txt
export BUILD_LANG=python27
source ../env.sh
export CC="gcc-4.7"
export CXX="g++-4.7"
cmake -DCMAKE_BUILD_TYPE=Debug -DWITH_MATLAB=OFF ..
EOF

cat <<EOF > build-matlab/run.sh
rm CMakeCache.txt
export BUILD_LANG=matlabR2014a
source ../env.sh
export MATLABRELEASE=R2014a
export MATLAB_ROOT=/matlab/
export CC="gcc-4.7"
export CXX="g++-4.7"
cmake -DCMAKE_BUILD_TYPE=Debug -DWITH_PYTHON=OFF -DWITH_MATLAB=ON ..
EOF                                               
