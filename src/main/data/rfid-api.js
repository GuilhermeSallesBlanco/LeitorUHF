let ultimoUIDRecebido = "";

let ultimaRespostaESP32 = Date.now();

async function verificarRFID()
{
    try
    {
        const resposta = await fetch("/rfid");

        ultimaRespostaESP32 = Date.now();

        esp32Status.textContent = "ONLINE";
        esp32Status.className = "status-online";

        const uid = (await resposta.text()).trim();

        if(uid === "")
        {
            return;
        }

        ultimoUIDRecebido = uid;

        processRFID(uid);
    }
    catch(error)
    {
        esp32Status.textContent = "OFFLINE";
        esp32Status.className = "status-offline";

        rfidStatus.textContent = "DESCONECTADO";
        rfidStatus.className = "status-offline";

        console.error(error);
    }
}

setInterval(verificarRFID, 1000);