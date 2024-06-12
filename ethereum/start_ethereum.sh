# Variables
DIRECTORY="go-ethereum-1.13.15"
SUBNET="102.0.1.0/24"
NETWORK="triton-cpp-example"
SERVICE_NAME="triton-cpp-ethereum"
IP="102.0.1.7"
PORT="8546"
NODE_DIRECTORY="../node"

COMMAND_INIT="geth --datadir /node init /node/genesis.json"

# Start
cd $DIRECTORY || { exit 1; }
docker network create --subnet $SUBNET $NETWORK

docker build -t $SERVICE_NAME . || { exit 1; }
docker run -d --rm --name $SERVICE_NAME --network $NETWORK --ip $IP -p $PORT:8546 -v $PWD/$NODE_DIRECTORY:/node $SERVICE_NAME || { exit 1; }
docker exec -d $SERVICE_NAME $COMMAND_INIT || { exit 1; }

sleep 5
echo "Start ethereum network."
docker exec -d $SERVICE_NAME geth --datadir /node --networkid 200 --port 30304 --http --http.api "personal,eth,web3,net,miner,admin,debug,txpool" --http.corsdomain "*" --http.addr "0.0.0.0" --http.port 8546 --authrpc.port 8552 --ws --ws.origins "*" --ws.api "eth,net,web3" --ws.port 8547 --unlock 0,1 --password /node/password --allow-insecure-unlock --syncmode "snap" --ipcdisable || { exit 1; }

sleep 5
echo "Start mining."
docker exec -d $SERVICE_NAME geth --exec "miner.setEtherbase(eth.accounts[0]); miner.start()" attach http://0.0.0.0:8546 || { exit 1; }