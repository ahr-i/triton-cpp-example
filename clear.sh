sudo docker stop ethereum-node
sudo docker stop triton

sudo rm -rf ethereum/node/geth
sudo rm -r ethereum/node/history
sudo rm -r ethereum/node/geth.ipc
sudo rm -rf gpu-node/stable-diffusion-v1-4-onnx
sudo rm -rf gpu-node/models

cd backend
make clean

cd ../gpu-node/manager
make clean

cd ../../service/gateway
make clean

cd ../health-checker
make clean

cd ../scheduler
make clean