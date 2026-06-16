const logsContainer = document.getElementById("logs");

const activeCards = document.getElementById("activeCards");

const historyContainer = document.getElementById("historyContainer");

const caseTitle = document.querySelector(".case-title");

const nextButton = document.getElementById("nextCaseButton");

const prevButton = document.getElementById("prevCaseButton");

const devSidebar = document.getElementById("devSidebar");

const devButton = document.getElementById("devButton");

const closeSidebar = document.getElementById("closeSidebar");

const employeeSelect = document.getElementById("employeeSelect");

const addEmployeeButton = document.getElementById("addEmployeeButton");

const autoSimulationCheckbox = document.getElementById("autoSimulation");

const clearConsoleButton = document.getElementById("clearConsoleButton");

const clearEmployeesButton = document.getElementById("clearEmployeesButton");

const esp32Status = document.getElementById("esp32Status");

const rfidStatus = document.getElementById("rfidStatus");

const esp32Ip = document.getElementById("esp32Ip");

const lastReadTime = document.getElementById("lastReadTime");

const lastReadTag = document.getElementById("lastReadTag");



/*
===========================================
ESTADO GLOBAL
===========================================
*/

let currentCaseIndex = 0;

const activeEmployees = [];
const accessHistory = [];
const RFID_TIMEOUT = 5000; // 5 segundos

let shouldAutoScrollEmployees = true;





/*
===========================================
ELEMENTOS SCROLL
===========================================
*/

const consoleContainer = document.querySelector(".logs-container");

const employeesContainer = document.querySelector(".right-panel");





/*
===========================================
CONTROLE SCROLL FUNCIONÁRIOS
===========================================
*/

employeesContainer.addEventListener("scroll", () => {

    const distanceFromBottom =
        employeesContainer.scrollHeight -
        employeesContainer.scrollTop -
        employeesContainer.clientHeight;

    shouldAutoScrollEmployees = distanceFromBottom < 80;
});





/*
===========================================
SIDEBAR DEV
===========================================
*/

devButton.addEventListener("click", () => {

    devSidebar.classList.add("open");
});

closeSidebar.addEventListener("click", () => {

    devSidebar.classList.remove("open");
});





/*
===========================================
POPULA SELECT
===========================================
*/

function populateEmployeeSelect() {

    employeeSelect.innerHTML = "";

    employees.forEach(employee => {

        const option = document.createElement("option");

        option.value = employee.uid;

        option.textContent = `${employee.nome} (${employee.tag})`;

        employeeSelect.appendChild(option);
    });
}

populateEmployeeSelect();





/*
===========================================
BOTÃO MOCK RFID
===========================================
*/

addEmployeeButton.addEventListener("click", () => {

    const selectedUID = employeeSelect.value;

    processRFID(selectedUID);
});





/*
===========================================
LIMPAR CONSOLE
===========================================
*/

clearConsoleButton.addEventListener("click", () => {

    logsContainer.innerHTML = "";

    addLog([
        `[SYSTEM] Console limpo manualmente`
    ], "normal");
});





/*
===========================================
LIMPAR FUNCIONÁRIOS
===========================================
*/

clearEmployeesButton.addEventListener("click", () => {

    activeEmployees.length = 0;

    renderCards();

    addLog([
        `[SYSTEM] Lista de funcionários resetada`
    ], "normal");
});





/*
===========================================
ATUALIZA TÍTULO
===========================================
*/

function updateCaseTitle() {

    caseTitle.textContent = `CASO ${cases[currentCaseIndex].id}`;
}





/*
===========================================
SCROLL CONSOLE
===========================================
*/

function scrollConsoleToBottom() {

    consoleContainer.scrollTop = consoleContainer.scrollHeight;
}





/*
===========================================
SCROLL FUNCIONÁRIOS
===========================================
*/

function scrollEmployeesToBottom() {

    employeesContainer.scrollTop = employeesContainer.scrollHeight;
}





/*
===========================================
ADICIONA LOG
===========================================
*/

function addLog(messages, type) {

    const log = document.createElement("div");

    log.classList.add("log-entry");

    const now = new Date();

    const timeString = now.toLocaleTimeString();

    const timestamp = document.createElement("div");

    timestamp.style.color = "#666";

    timestamp.style.fontSize = "0.85rem";

    timestamp.style.marginBottom = "6px";

    timestamp.style.fontFamily = "monospace";

    timestamp.textContent = `[${timeString}]`;

    log.appendChild(timestamp);

    messages.forEach(msg => {

        const line = document.createElement("div");

        line.classList.add(type);

        line.textContent = msg;

        log.appendChild(line);
    });

    logsContainer.appendChild(log);

    requestAnimationFrame(() => {

        scrollConsoleToBottom();

        setTimeout(() => {

            scrollConsoleToBottom();

        }, 50);

    });
}





/*
===========================================
RENDERIZA CARDS
===========================================
*/

function renderCards() {

    activeCards.innerHTML = "";

    activeEmployees.forEach(employee => {

        let formattedTime = "--:--:--";





        /*
        =======================================
        APENAS AUTORIZADOS CONTAM TEMPO
        =======================================
        */

        if (employee.allowed) {

            const elapsed = Date.now() - employee.startTime;

            const totalSeconds = Math.floor(elapsed / 1000);

            const hours = String(Math.floor(totalSeconds / 3600)).padStart(2, "0");

            const minutes = String(Math.floor((totalSeconds % 3600) / 60)).padStart(2, "0");

            const seconds = String(totalSeconds % 60).padStart(2, "0");

            formattedTime = `${hours}:${minutes}:${seconds}`;
        }

        const card = document.createElement("div");

        card.classList.add("employee-card");

        if (!employee.allowed) {

            card.classList.add("denied");
        }

        card.innerHTML = `

            <div class="employee-name">
                ${employee.nome}
            </div>

            <div class="employee-zone">
                ${employee.currentZone}
            </div>

            <div class="employee-timer">
                ${formattedTime}
            </div>

            <div class="employee-status ${employee.allowed ? "status-approved" : "status-denied"}">
                ${employee.allowed ? "AUTORIZADO" : "NEGADO"}
            </div>

        `;

        activeCards.appendChild(card);
    });

    if (shouldAutoScrollEmployees) {

        requestAnimationFrame(() => {

            scrollEmployeesToBottom();

            setTimeout(() => {

                scrollEmployeesToBottom();

            }, 50);

        });
    }
}





/*
===========================================
PROCESSA RFID
===========================================
*/

function processRFID(uid)
{
    console.log(
        "PROCESSANDO RFID:",
        uid,
        new Date().toLocaleTimeString()
    );
    
    console.log("RFID RECEBIDO:", uid);

    const employee =
        employees.find(emp => emp.uid === uid);

    updateSystemStatus(uid);

    const currentCase =
        cases[currentCaseIndex];

    if (!employee)
    {
        addLog([
            `[ERRO] Tag desconhecida`,
            `[ERRO] UID ${uid}`
        ], "error");

        return;
    }

    const allowed =
        employee.permissoes.includes(currentCase.id);

    const existingEmployee =
        activeEmployees.find(emp => emp.uid === employee.uid);

    if (existingEmployee)
    {
        existingEmployee.lastSeen = Date.now();

        console.log(
            "PRESENÇA RENOVADA:",
            employee.nome
        );

        return;
    }

    console.log(
        "NOVA ENTRADA:",
        employee.nome
    );

    accessHistory.push({
        nome: employee.nome,
        cargo: employee.cargo,
        zona: currentCase.nome,
        allowed,
        time: new Date().toLocaleTimeString()
    });

    /*
    ===========================================
    CASO 1
    ===========================================
    */

    if (currentCase.id === 1)
    {
        if (allowed)
        {
            addLog([
                `[MONITORAMENTO] ${employee.nome} identificado`,
                `[CLP] Dados enviados com sucesso`,
                `[CATRACA] Acesso liberado`
            ], "success");
        }
        else
        {
            addLog([
                `[MONITORAMENTO] ${employee.nome} sem autorização`,
                `[CATRACA] Entrada negada`,
                `[CATRACA] Porta permaneceu bloqueada`
            ], "error");
        }
    }

    /*
    ===========================================
    CASO 2
    ===========================================
    */

    else if (currentCase.id === 2)
    {
        if (allowed)
        {
            addLog([
                `[MÁQUINA] ${employee.nome} autorizado`,
                `[SENSOR] Máquina permanece ativa`
            ], "success");
        }
        else
        {
            addLog([
                `[RISCO] ${employee.nome} entrou sem autorização`,
                `[CLP] Desligamento automático acionado`,
                `[MÁQUINA] Estado seguro ativado`
            ], "error");
        }
    }

    /*
    ===========================================
    CASO 3
    ===========================================
    */

    else if (currentCase.id === 3)
    {
        if (allowed)
        {
            addLog([
                `[TREINAMENTO] ${employee.nome} autorizado`,
                `[SISTEMA] Acesso liberado`
            ], "success");
        }
        else
        {
            addLog([
                `[TREINAMENTO] Funcionário sem capacitação`,
                `[CLP] Evento crítico registrado`,
                `[SISTEMA] Máquina desligada preventivamente`
            ], "error");
        }
    }

    /*
    ===========================================
    CASO 4
    ===========================================
    */

    else if (currentCase.id === 4)
    {
        addLog([
            `[CAPACIDADE] ${employee.nome} entrou na área`,
            `[SENSOR] Total atual: ${activeEmployees.length + 1} funcionários`
        ], "normal");

        if (activeEmployees.length + 1 > 3)
        {
            addLog([
                `[ALERTA] Limite de funcionários excedido`,
                `[SEGURANÇA] Área acima da capacidade máxima`
            ], "error");
        }
    }

    /*
    ===========================================
    ADICIONA FUNCIONÁRIO
    ===========================================
    */

    activeEmployees.push({

        ...employee,

        allowed,

        currentZone: currentCase.nome,

        startTime: allowed ? Date.now() : null,

        lastSeen: Date.now()
    });

    renderCards();
    renderHistory();
}





/*
===========================================
ALTERAÇÃO DE CASOS
===========================================
*/

function nextCase() {

    currentCaseIndex++;

    if (currentCaseIndex >= cases.length) {

        currentCaseIndex = 0;
    }

    updateCaseTitle();

    const currentCase = cases[currentCaseIndex];

    addLog(currentCase.descricao, "normal");
}





function previousCase() {

    currentCaseIndex--;

    if (currentCaseIndex < 0) {

        currentCaseIndex = cases.length - 1;
    }

    updateCaseTitle();

    const currentCase = cases[currentCaseIndex];

    addLog(currentCase.descricao, "normal");
}





/*
===========================================
BOTÕES
===========================================
*/

nextButton.addEventListener("click", nextCase);

prevButton.addEventListener("click", previousCase);


/*
===========================================
INICIALIZAÇÃO
===========================================
*/

updateCaseTitle();

addLog(cases[currentCaseIndex].descricao, "normal");

setInterval(renderCards, 250);

scrollConsoleToBottom();

scrollEmployeesToBottom();


function monitorEmployeePresence()
{
    const now = Date.now();

    for(let i = activeEmployees.length - 1; i >= 0; i--)
    {
        const employee = activeEmployees[i];

        if(now - employee.lastSeen > RFID_TIMEOUT)
        {
            addLog([
                `[MONITORAMENTO] ${employee.nome} saiu da zona`,
                `[RFID] Tag não detectada por ${RFID_TIMEOUT / 1000}s`
            ], "normal");

            accessHistory.push({
                nome: employee.nome,
                cargo: employee.cargo,
                zona: employee.currentZone,
                allowed: employee.allowed,
                exited: true,
                time: new Date().toLocaleTimeString()
            });

            activeEmployees.splice(i, 1);

            renderCards();
            renderHistory();
        }
    }
}


/*
===========================================
AUTO SIMULAÇÃO
===========================================
*/

autoSimulationCheckbox.checked = false;

function startAutomaticSimulation() {

    setTimeout(() => {

        if (autoSimulationCheckbox.checked) {

            processRFID("E280689400001");
        }

    }, 2000);

    setTimeout(() => {

        if (autoSimulationCheckbox.checked) {

            processRFID("E280689400002");
        }

    }, 5000);

    setTimeout(() => {

        if (autoSimulationCheckbox.checked) {

            nextCase();
        }

    }, 8000);

    setTimeout(() => {

        if (autoSimulationCheckbox.checked) {

            processRFID("E280689400003");
        }

    }, 11000);
}

startAutomaticSimulation();

function updateSystemStatus(uid)
{
    const now = new Date();

    lastReadTime.textContent =
        now.toLocaleTimeString();

    lastReadTag.textContent = uid;
}

function monitorConnection()
{
    const agora = Date.now();

    if(agora - ultimaRespostaESP32 > 5000)
    {
        esp32Status.textContent = "OFFLINE";
        esp32Status.className = "status-offline";

        rfidStatus.textContent = "AGUARDANDO";
        rfidStatus.className = "status-offline";
    }
}

setInterval(monitorConnection, 1000);
setInterval(monitorEmployeePresence, 500);

function renderHistory()
{
    historyContainer.innerHTML = "";

    const reversed =
        [...accessHistory].reverse();

    reversed.forEach(entry =>
    {
        const item =
            document.createElement("div");

        item.classList.add("history-item");

        let statusClass = "";
        let statusText = "";

        if(entry.exited)
        {
            statusClass = "status-neutral";
            statusText = "SAIU";
        }
        else if(entry.allowed)
        {
            statusClass = "status-approved";
            statusText = "AUTORIZADO";
        }
        else
        {
            statusClass = "status-denied";
            statusText = "NEGADO";
        }

        item.innerHTML = `

            <div class="history-time">
                ${entry.time}
            </div>

            <div class="history-name">
                ${entry.nome}
            </div>

            <div class="${statusClass}">
                ${statusText}
            </div>

        `;

        historyContainer.appendChild(item);
    });
}