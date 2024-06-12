const express = require('express');
const Web3 = require('web3').Web3;

const app = express();
const port = 8083;

/* ----- Setting ----- */
/* HTTP-rpc Address */
const web3 = new Web3(new Web3.providers.HttpProvider('http://102.0.1.7:8546'));	// HTTP-RPC Address

/* Sender Address & Private Key */
const senderAddr = '0xd5bE5156a8Fa099dd54a45eA4B55c6D1C736E4ea';			// Address of the account sending funds
const privateKey = 'b378e6296fdfebe4365b778c0ee75025ed9f277bac391c4c67728cf34e80d861';	// Private key of the sender address

/* Receiver Address */
const receiverAddr = '0x58C733A7e2a7eb88937d2ffa84B2F294db04D3e5';			// Address of the account receiving funds

/* Transaction Count */
const transactionCount = 1;   // Number of transactions to send

/* ----- Send Transaction ----- */
async function sendTransaction(nonce) {
    const gasPrice = await web3.eth.getGasPrice();

    const transaction = {
        from: senderAddr,
        to: receiverAddr,
        value: web3.utils.toWei('1', 'ether'),
        gasPrice: gasPrice,
        gas: 21000,
        nonce: nonce
    };
    const signedTransaction = await web3.eth.accounts.signTransaction(transaction, privateKey);

    return web3.eth.sendSignedTransaction(signedTransaction.rawTransaction);
}

/* ----- Measure TPS ----- */
async function measureTPS() {
    let startNonce = await web3.eth.getTransactionCount(senderAddr);
    startNonce = Number(startNonce);

    console.log("Start Nonce: " + startNonce);
    console.log("End Nonce: " + (startNonce + transactionCount - 1));

    const batchSize = 1;
    const batchCount = Math.ceil(transactionCount / batchSize);
    let startTime, endTime;
    let complete = 0;

    for (let batch = 0; batch < batchCount; batch++) {
        const promises = [];
        for (let i = 0; i < batchSize && (batch * batchSize + i) < transactionCount; i++) {
            promises.push(sendTransaction(startNonce + batch * batchSize + i));
        }

        if (batch === 0) {
            startTime = Date.now();
        }

        await Promise.all(promises);
        await new Promise(resolve => setTimeout(resolve, 2000));

        complete += batchSize;
        console.log(complete + " transactions completed.");
    }

    endTime = Date.now();

    const resultTime = (endTime - startTime) / 1000 - (batchCount - 1) * 2;
    const tps = transactionCount / resultTime;

    console.log(`TPS: ${tps}`);
}

app.post('/pay', async (req, res) => {
    try {
        await measureTPS();
        res.status(200).send('Transaction completed and TPS measured.');
    } catch (error) {
        console.error(error);
        res.status(500).send('An error occurred while processing the transaction.');
    }
});

app.listen(port, () => {
    console.log(`Server running at http://102.0.1.6:${port}`);
});
