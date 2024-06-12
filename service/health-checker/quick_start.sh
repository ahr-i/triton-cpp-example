# Variables
SERVICE_NAME="triton-cpp-health"
SUBNET="102.0.1.0/24"
NETWORK="triton-cpp-example"
IP="102.0.1.5"

# Start
docker build -t $SERVICE_NAME . || { exit 1; }

docker network create --subnet $SUBNET $NETWORK

docker run -it --rm --name $SERVICE_NAME --network $NETWORK --ip $IP $SERVICE_NAME || { exit 1; }