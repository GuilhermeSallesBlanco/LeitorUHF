Esta explicação veio do código de Playful Technology. Ela elabora em como que comandos devem ser escritos e enviados ao leitor RFID para que ele realize certas ações.

# Texto original:

Commands sent to the reader, and responses received back, are sent as data frames, e.g.
Header | Type | Command | ParamLength (2bytes) | Parameter(s) | Checksum | End
  AA   |  00  |   07    |      00 03           |   04 02 05   |    15    | DD

Frames always start with the header value 0xAA
Type indicates a command to the reader (0x00), or a response (0x01), or notification (0x02) back from it
Command is the instruction to be performed, or the response from that instruction
ParamLength gives 2-byte (MSB then LSB) number of parameters being passed in the frame
Params may be zero or more
Checksum is the LSB of the sum of bytes from the type to the last instruction parameter (i.e. excluding Frame Header)
Frames always end with the tail value 0xDD

# Tradução (própria):

Comandos enviados ao leitor, e respostas recebidas de volta, são enviados como data frames. Por exemplo:
Header | Type | Command | ParamLength (2bytes) | Parameter(s) | Checksum | End
  AA   |  00  |   07    |      00 03           |   04 02 05   |    15    | DD

Frames sempre começam com o valor haeder 0xAA
Type indica um comando ao leitor (0x00), uma resposta (0x01), ou uma notificação (0x02) de volta do leitor
Command é a instrução a ser realizada, ou a resposta daquela instrução
ParamLength dá o número de parâmetros no frame, em 2 bytes (MSB -> LSB)
Params podem ser zero ou mais
Checksum é o LSB da soma dos bytes do tipo até o último parâmetro da instrução (ou seja, excluindo o Frame Header)
Frames sempre terminam com o valor tail 0xDD

# Alguns valores de comando:
- Frame header = 0xAA
- Frame tail = 0xDD
- Type: 0x00 (comando), 0x01 (resposta), 0x02 (notificação)
- GetModuleInfo = 0x03
- SinglePollInstruction = 0x22
- MultiplePollInstruction = 0x27
- StopMultiplePoll = 0x28
- SetSelectParameter = 0x0C
- GetSelectParameter = 0x0B
- SetSendSelectInstruction = 0x12
- ReadLabel = 0x39
- WriteLabel = 0x49
- LockLabel = 0x82
- KillTag = 0x65
...

Os outros comandos se encontram detalhados no código-fonte do repositório de Playful Technology, especificamente no código R200.h (definição de classe). Podem ser encontrados em https://github.com/playfultechnology/arduino-rfid-R200/blob/main/R200/R200.h.
