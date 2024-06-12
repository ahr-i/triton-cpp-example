if [ ! -d "$PWD/models" ]; then
    bash stable_diffusion.sh
fi

# Variables
SERVICE_NAME="triton-cpp-triton"
SUBNET="102.0.1.0/24"
NETWORK="triton-cpp-example"
IP="102.0.1.9"

# Start
docker network create --subnet $SUBNET $NETWORK

docker build -t $SERVICE_NAME ./triton || { exit 1; }
docker run -d --rm --name $SERVICE_NAME --gpus all --network $NETWORK --ip 102.0.1.9 -v $PWD/models:/models $SERVICE_NAME tritonserver --model-repository /models || { exit 1; }