// stdafx.h: file di inclusione per file di inclusione del sistema standard
// o file di inclusione specifici del progetto usati di frequente, ma
// modificati raramente
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Escludere gli elementi usati raramente dalle intestazioni di Windows
// File di intestazione di Windows

// File di intestazione Runtime C
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// fare riferimento qui alle intestazioni aggiuntive richieste dal programma
#include <stdio.h>
#include <fcntl.h>
#include <fstream>
#include <io.h>
#include <iostream>
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <windows.h>