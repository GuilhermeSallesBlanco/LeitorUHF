const employees = [

    {
        uid: "8069150000501D63E66C4722",
        tag: "TAG-001",
        nome: "Carlos Silva",
        cargo: "Administrador",
        permissao: "ADMIN",
        permissoes: [1, 2, 3, 4]
    },

    {
        uid: "8069150000401D63E670477E",
        tag: "TAG-002",
        nome: "Fernanda Rocha",
        cargo: "Supervisor",
        permissao: "SUPERVISOR",
        permissoes: [1, 2, 3]
    },

    {
        uid: "8069150000501D63E67447A8",
        tag: "TAG-003",
        nome: "Julio Mendes",
        cargo: "Supervisor",
        permissao: "SUPERVISOR",
        permissoes: [1, 2, 3]
    },

    {
        uid: "8069150000501D63E67847ED",
        tag: "TAG-004",
        nome: "Ana Costa",
        cargo: "Funcionário",
        permissao: "FUNCIONARIO",
        permissoes: [1, 4]
    },

    {
        uid: "8069150000401D63E67C473B",
        tag: "TAG-005",
        nome: "Rafael Oliveira",
        cargo: "Funcionário",
        permissao: "FUNCIONARIO",
        permissoes: [1, 3, 4]
    },

    {
        uid: "8069150000401D63E680476D",
        tag: "TAG-006",
        nome: "Visitante",
        cargo: "Visitante",
        permissao: "VISITANTE",
        permissoes: []
    }
];





const cases = [

    {
        id: 1,

        nome: "MONITORAMENTO",

        descricao: [

            "[CASO 1] Sistema de monitoramento ativo",

            "[CLP] Todos os acessos serão registrados",

            "[CLP] Informações de presença sendo enviadas"
        ]
    },





    {
        id: 2,

        nome: "MÁQUINA PERIGOSA",

        descricao: [

            "[CASO 2] Área de risco monitorada",

            "[SENSOR] Máquina protegida por sistema RFID",

            "[CLP] Máquina será desligada em acessos não autorizados"
        ]
    },





    {
        id: 3,

        nome: "SEM AUTORIZAÇÃO",

        descricao: [

            "[CASO 3] Controle de treinamento ativo",

            "[SENSOR] Sistema verificando permissões do funcionário",

            "[CLP] Ações automáticas variam conforme o nível de risco"
        ]
    },





    {
        id: 4,

        nome: "LIMITE DE PESSOAS",

        descricao: [

            "[CASO 4] Controle de capacidade ativo",

            "[SENSOR] Quantidade máxima de funcionários monitorada",

            "[ALERTA] Sistema emitirá aviso se o limite for excedido"
        ]
    }
];