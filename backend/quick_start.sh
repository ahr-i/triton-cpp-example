# Variables
SERVICE_NAME="triton-cpp-backend"
SUBNET="102.0.1.0/24"
NETWORK="triton-cpp-example"
IP="102.0.1.2"
PORT="80"

# Start
docker build -t $SERVICE_NAME . || { exit 1; }

docker network create --subnet $SUBNET $NETWORK

docker run -it --rm --name $SERVICE_NAME --network $NETWORK --ip 102.0.1.2 -p $PORT:80 $SERVICE_NAME || { exit 1; }