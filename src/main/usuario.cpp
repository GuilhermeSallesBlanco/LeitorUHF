// Implementações das funções relacionadas aos usuários, cenários e permissões

#include "usuario.h"
#include <Arduino.h>

bool compararUID(const uint8_t *uid1, const uint8_t *uid2){
    return memcmp(uid1, uid2, UID_LENGTH) == 0; // Retorna true se os UIDs forem iguais, false caso contrário
}


