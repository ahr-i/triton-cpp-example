# Variables
SERVICE_NAME="triton-cpp-token"
SUBNET="102.0.1.0/24"
NETWORK="triton-cpp-example"
IP="102.0.1.6"

# Start
docker build -t $SERVICE_NAME . || { exit 1; }

docker network create --subnet $SUBNET $NETWORK

docker run -d --rm --name $SERVICE_NAME --network $NETWORK --ip $IP -p 8083:8083 $SERVICE_NAME || { exit 1; }