# CPU performance test

./test.sh


# XNNPACK

Install bazel to compile XNNPACK

```bash
sudo apt install apt-transport-https curl gnupg
curl -fsSL https://bazel.build/bazel-release.pub.gpg | gpg --dearmor > bazel.gpg
sudo mv bazel.gpg /etc/apt/trusted.gpg.d/
echo "deb [arch=amd64] https://storage.googleapis.com/bazel-apt stable jdk1.8" | sudo tee /etc/apt/sources.list.d/bazel.list
sudo apt update && sudo apt install bazel
```

Clone XNNPACK

```bash
git clone https://github.com/google/XNNPACK
```

Build XNNPACK
```bash
pushd XNNPACK
./scripts/build-local.sh
popd
```
