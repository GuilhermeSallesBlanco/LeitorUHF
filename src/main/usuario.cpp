// Implementações das funções relacionadas aos usuários, cenários e permissões

#include "usuario.h"
#include <Arduino.h>

bool compararUID(const uint8_t *uid1, const uint8_t *uid2){
    return memcmp(uid1, uid2, UID_LENGTH) == 0; // Retorna true se os UIDs forem iguais, false caso contrário
}

// Podemos colocar outras funções, como imprimirUID(), mas acho que podemos deixar isso para depois ou apenas no front end.
// O máximo que o back end deve fazer, por enquanto, é comparar os UIDs lidos com os UIDs dos usuários cadastrados e, caso haja uma correspondência, enviar
// o nome do usuário, a UID e a permissão para o front end. 

// Caso a comparação dê negativa, o back end pode enviar uma resposta indicando que o usuário é desconhecido, ou seja, que a tag lida não corresponde a nenhum usuário cadastrado.
