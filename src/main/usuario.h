// Definição da struct, enums e funções relacionadas aos usuários, cenários e permissões

#ifndef USUARIO_H
#define USUARIO_H

#include <stdint.h>
#include <Arduino.h>

#define UID_LENGTH 12

enum NivelPermissao{
    PERM_VISITANTE = 0,
    PERM_FUNCIONARIO = 1,
    PERM_SUPERVISOR = 2,
    PERM_ADMIN = 3
};

struct Usuario{
    uint8_t uid[UID_LENGTH]; // UID da tag do usuário
    char nome[100]; // Nome do usuário
    char cargo[50]; // Cargo do usuário
    NivelPermissao permissao; // Nível de permissão do usuário

    uint32_t contadorLeituras = 0;
};

bool compararUID(const uint8_t *uid1, const uint8_t *uid2);

#endif