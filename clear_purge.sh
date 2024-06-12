docker stop triton-cpp-backend
docker stop triton-cpp-gateway
docker stop triton-cpp-scheduler
docker stop triton-cpp-health
docker stop triton-cpp-token
docker stop triton-cpp-ethereum
docker stop triton-cpp-manager
docker stop triton-cpp-triton

rm -rf ethereum/node/geth
rm -r ethereum/node/history
rm -rf gpu-node/stable-diffusion-v1-4-onnx
rm -rf gpu-node/models

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