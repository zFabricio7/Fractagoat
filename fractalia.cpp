#include "raylib.h"
#include "fractalia.h"
#include <time.h>
#include <stdlib.h>

#define STAR_COUNT 300

static float starX[STAR_COUNT];
static float starY[STAR_COUNT];
static float starSpeed[STAR_COUNT];
int estrellas_inicializadas = 0;
int randomizado = 0;
Pantalla Pantalla_Actual;
Pantalla nivel_actual;
Font fuenteLilita;
Sound pair;

void estrellas(float dt, int pantalla_ancho, int pantalla_alto);
void PantallaMenu(int pantalla_ancho, int pantalla_alto);
void tutorial();
void nivel1();
void nivel2();
void nivel3();
void pantalla_final(int tiempo_restante, int vida_rival, int nivel, bool *resetear);
void pantalla_creditos();

int main(void)
{
    srand(time(NULL));
    const int pantalla_ancho = 1920;
    const int pantalla_alto = 1080;

    InitWindow(pantalla_ancho, pantalla_alto, "FRACTALIA");
    InitAudioDevice();
    SetTargetFPS(60);

    Texture2D logo = LoadTexture("assets/images/LOGO FRACTALIA.png");
    Music musicaMenu = LoadMusicStream("assets/music/musica_menu.mp3");
    Music musicaTuto = LoadMusicStream("assets/music/tutorial.mp3");
    Music musicaLvl1 = LoadMusicStream("assets/music/level_1.mp3");
    Music musicaLvl2 = LoadMusicStream("assets/music/level_2.mp3");
    Music musicaLvl3 = LoadMusicStream("assets/music/level_3.mp3");
    fuenteLilita = LoadFont("assets/fonts/LilitaOneRegular.ttf");
    pair = LoadSound("assets/sounds/pair.mp3");
    PlayMusicStream(musicaMenu);
    SetSoundVolume(pair, 0.7f);
    SetMusicVolume(musicaMenu, 0.4f);
    SetMusicVolume(musicaTuto, 0.4f);
    SetMusicVolume(musicaLvl1, 0.4f);
    SetMusicVolume(musicaLvl2, 0.4f);
    SetMusicVolume(musicaLvl3, 0.4f);

    Pantalla_Actual = INICIO;

    while (!WindowShouldClose())
    {
        UpdateMusicStream(musicaMenu);
        UpdateMusicStream(musicaTuto);
        UpdateMusicStream(musicaLvl1);
        UpdateMusicStream(musicaLvl2);
        UpdateMusicStream(musicaLvl3);

        float dt = GetFrameTime();

        BeginDrawing();
        ClearBackground(BLACK);

        switch (Pantalla_Actual)
        {
        case INICIO:
            StopMusicStream(musicaTuto);
            StopMusicStream(musicaLvl1);
            StopMusicStream(musicaLvl2);
            StopMusicStream(musicaLvl3);

            if (!IsMusicStreamPlaying(musicaMenu))
                PlayMusicStream(musicaMenu);

            estrellas(dt, pantalla_ancho, pantalla_alto);
            PantallaMenu(pantalla_ancho, pantalla_alto);
            break;

        case TUTORIAL:
            StopMusicStream(musicaMenu);
            StopMusicStream(musicaLvl1);
            StopMusicStream(musicaLvl2);
            StopMusicStream(musicaLvl3);

            if (!IsMusicStreamPlaying(musicaTuto))
                PlayMusicStream(musicaTuto);

            estrellas(dt, pantalla_ancho, pantalla_alto);
            tutorial();
            break;

        case NIVEL1:
            StopMusicStream(musicaMenu);
            StopMusicStream(musicaTuto);
            StopMusicStream(musicaLvl2);
            StopMusicStream(musicaLvl3);

            if (!IsMusicStreamPlaying(musicaLvl1))
                PlayMusicStream(musicaLvl1);

            estrellas(dt, pantalla_ancho, pantalla_alto);
            nivel1();
            break;

        case NIVEL2:
            StopMusicStream(musicaMenu);
            StopMusicStream(musicaTuto);
            StopMusicStream(musicaLvl1);
            StopMusicStream(musicaLvl3);

            if (!IsMusicStreamPlaying(musicaLvl2))
                PlayMusicStream(musicaLvl2);

            estrellas(dt, pantalla_ancho, pantalla_alto);
            nivel2();
            break;

        case NIVEL3:
            StopMusicStream(musicaMenu);
            StopMusicStream(musicaTuto);
            StopMusicStream(musicaLvl1);
            StopMusicStream(musicaLvl2);

            if (!IsMusicStreamPlaying(musicaLvl3))
                PlayMusicStream(musicaLvl3);

            estrellas(dt, pantalla_ancho, pantalla_alto);
            nivel3();
            break;
        }

        EndDrawing();
    }

    UnloadTexture(logo);
    UnloadMusicStream(musicaMenu);
    UnloadFont(fuenteLilita);
    UnloadSound(pair);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

void PantallaMenu(int screenWidth, int screenHeight)
{
    static int cargado = 0;
    static Texture2D logo;

    if (cargado == 0)
    {
        logo = LoadTexture("assets/images/LOGO FRACTALIA.png");
        cargado = 1;
    }

    float targetWidth = screenWidth * 0.40f;
    float scale = targetWidth / logo.width;

    float posX = (screenWidth - logo.width * scale) / 2;
    float posY = -20;

    Vector2 mouse = GetMousePosition();
    bool click = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    DrawTextureEx(logo, (Vector2){posX, posY}, 0.0f, scale, WHITE);

    int btnWidth = 420;
    int btnHeight = 100;
    float x = screenWidth / 2 - btnWidth / 2;

    Rectangle btnJugar = {x, 650, (float)btnWidth, (float)btnHeight};
    Rectangle btnSalir = {x, 800, (float)btnWidth, (float)btnHeight};

    Color colorJugar = CheckCollisionPointRec(mouse, btnJugar) ? GOLD : YELLOW;
    Color colorSalir = CheckCollisionPointRec(mouse, btnSalir) ? VIOLET : PURPLE;

    DrawRectangleRounded(btnJugar, 0.3f, 12, colorJugar);
    DrawRectangleRounded(btnSalir, 0.3f, 12, colorSalir);

    DrawTextEx(fuenteLilita, "JUGAR", (Vector2){btnJugar.x + btnJugar.width / 2 - MeasureTextEx(fuenteLilita, "JUGAR", 65, 2).x / 2, btnJugar.y + btnJugar.height / 2 - 65 / 2}, 65, 2, BLACK);
    DrawTextEx(fuenteLilita, "SALIR", (Vector2){btnSalir.x + btnSalir.width / 2 - MeasureTextEx(fuenteLilita, "SALIR", 65, 2).x / 2, btnSalir.y + btnSalir.height / 2 - 65 / 2}, 65, 2, BLACK);

    if (CheckCollisionPointRec(mouse, btnSalir) && click)
    {
        CloseWindow();
    }

    if (CheckCollisionPointRec(mouse, btnJugar) && click)
    {
        Pantalla_Actual = TUTORIAL;
    }
}

void tutorial()
{
    nivel_actual = TUTORIAL;
    static Carta tablero[6];
    static int randomizado = 0;
    Vector2 mouse = GetMousePosition();
    bool click = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    static int primera_carta = -1;
    static int segunda_carta = -1;
    static float tiempo_espera = 0.0f; // para ocultar tras fallo (jugador o bot)
    bool click_procesado = false;
    static int cargado = 0;
    static Texture2D fract;
    static int clicks = 0;
    static bool inicio = 0;
    // Eliminado: static int pares_encontrados = 0;
    // Eliminado: static int vida_rival = 3;
    static float tiempo_restante = 90.0f;
    static bool turno_jugador = true;
    static bool turno_bot = false;
    static bool debe_resetear = false;
    static float tiempo_accion_bot = 0.0f; // pausas del bot entre acciones
    static int intentos_fallidos = 0;      // (ya no se usa para cambiar turno)
    static float tiempoMensaje = 0.0f;     // Para los mensajes
    static const char frases[14][20] = {"MUY BIEN!", "EXCELENTE!", "GENIAL!", "PERFECTO!", "WOW!", "INCREIBLE!", "FANTASTICO!", "SUPER!", "IMPACTANTE!", "ASOMBROSO!", "BRILLANTE!", "SORPRENDENTE!", "LO LOGRASTE!", "MAGNIFICO!"};
    static const char *fraseActual = NULL;
    static bool juego_terminado = false; // cuando true mostramos la pantalla final y paramos la lógica
    static int ganador = -1;             // 1 = jugador, 0 = bot, 2 = empate

    // Variables del bot
    static int primera_bot = -1;
    static int segunda_bot = -1;
    const int totalCartas = 6;
    const int totalPares = totalCartas / 2;
    static int pares_jugador = 0;
    static int pares_bot = 0;

    if (debe_resetear)
    {
        randomizado = 0;
        primera_carta = -1;
        segunda_carta = -1;
        tiempo_espera = 0.0f;
        clicks = 0;
        inicio = 0;
        tiempo_restante = 90.0f;
        turno_jugador = true;
        turno_bot = false;
        tiempo_accion_bot = 0.0f;
        primera_bot = -1;
        segunda_bot = -1;
        debe_resetear = false;
        fraseActual = NULL;
        tiempoMensaje = 0.0f;
        pares_jugador = 0;
        pares_bot = 0;
        juego_terminado = false;
        ganador = -1;
    }

    if (cargado == 0)
    {
        fract = LoadTexture("assets/images/sprite_fract.png");
        cargado = 1;
    }

    if (!randomizado)
    {
        randomizar_tutorial(tablero, totalCartas);
        randomizado = 1;
    }

    // LÓGICA DE FIN DE JUEGO TEMPRANA (MOVIMIENTO Y CORRECCIÓN)
    if ((pares_jugador + pares_bot >= totalPares) || (tiempo_restante <= 0.0f))
    {
        juego_terminado = true;
    }

    if (juego_terminado)
    {
        // 1. Determinar el ganador final basado en los pares
        if (pares_jugador > pares_bot)
            ganador = 1; // Jugador gana
        else if (pares_bot > pares_jugador)
            ganador = 0; // Bot gana
        else
            ganador = 2; // Empate (puedes usar 2, o -1 si tu pantalla_final lo requiere)

        // Nota: El parámetro "vida_rival" de pantalla_final ya no se usa, pero
        // es posible que tu función lo espere. Si me la pasas, lo adapto.
        pantalla_final(tiempo_restante, 0, ganador, &debe_resetear);
        return; // SALIMOS de tutorial() inmediatamente
    }

    if (inicio == 0)
    {
        DrawTextureEx(fract, (Vector2){760, 680}, 0.0f, 0.5f, WHITE);
        DrawCircle(1065, 835, 15, WHITE);
        DrawCircle(1080, 820, 22, WHITE);

        Rectangle globo = {680, 340, 560, 370};
        DrawRectangleRounded(globo, 0.35f, 12, WHITE);
        DrawRectangleRoundedLines(globo, 0.35f, 12, BLACK);
        DrawCircleLines(1065, 835, 15, BLACK);
        DrawCircleLines(1080, 820, 22, BLACK);

        if (click)
        {
            if (clicks < 2)
            {
                clicks++;
            }
            else
            {
                inicio = true;
            }
        }

        float baseY = globo.y + 60;

        switch (clicks)
        {
        case 0:
            DrawTextEx(fuenteLilita, "BIENVENIDO AL REINO DE FRACTALIA!", (Vector2){globo.x + globo.width / 2 - MeasureTextEx(fuenteLilita, "BIENVENIDO AL REINO DE FRACTALIA!", 28, 2).x / 2, baseY}, 28, 2, BLACK);
            DrawTextEx(fuenteLilita, "NECESITO TU AYUDA PARA", (Vector2){globo.x + globo.width / 2 - MeasureTextEx(fuenteLilita, "NECESITO TU AYUDA PARA", 28, 2).x / 2, baseY + 65}, 28, 2, BLACK);
            DrawTextEx(fuenteLilita, "RESOLVER LOS MEMORAMAS Y ASI", (Vector2){globo.x + globo.width / 2 - MeasureTextEx(fuenteLilita, "RESOLVER LOS MEMORAMAS Y ASI", 28, 2).x / 2, baseY + 110}, 28, 2, BLACK);
            DrawTextEx(fuenteLilita, "SALVAR EL REINO", (Vector2){globo.x + globo.width / 2 - MeasureTextEx(fuenteLilita, "SALVAR EL REINO", 28, 2).x / 2, baseY + 155}, 28, 2, BLACK);
            break;

        case 1:
            DrawTextEx(fuenteLilita, "ES UN JUEGO POR TURNOS", (Vector2){globo.x + globo.width / 2 - MeasureTextEx(fuenteLilita, "ES UN JUEGO POR TURNOS", 28, 2).x / 2, baseY}, 28, 2, BLACK);
            DrawTextEx(fuenteLilita, "TU INTENTAS HACER UN PAR", (Vector2){globo.x + globo.width / 2 - MeasureTextEx(fuenteLilita, "TU INTENTAS HACER UN PAR", 28, 2).x / 2, baseY + 45}, 28, 2, BLACK);
            DrawTextEx(fuenteLilita, "SI ACERTAS SIGUES,", (Vector2){globo.x + globo.width / 2 - MeasureTextEx(fuenteLilita, "SI ACERTAS SIGUES,", 28, 2).x / 2, baseY + 90}, 28, 2, BLACK);
            DrawTextEx(fuenteLilita, "SI FALLAS LE TOCA AL BOT", (Vector2){globo.x + globo.width / 2 - MeasureTextEx(fuenteLilita, "SI FALLAS LE TOCA AL BOT", 28, 2).x / 2, baseY + 135}, 28, 2, BLACK);
            break;

        case 2:
            DrawTextEx(fuenteLilita, "TU PUEDES!", (Vector2){globo.x + globo.width / 2 - MeasureTextEx(fuenteLilita, "TU PUEDES!", 100, 2).x / 2, baseY + 75}, 100, 2, BLACK);

            break;
        }

        DrawTextEx(fuenteLilita, "CLICK PARA CONTINUAR", (Vector2){globo.x + globo.width / 2 - MeasureTextEx(fuenteLilita, "CLICK PARA CONTINUAR", 30, 2).x / 2, globo.y + globo.height - 50}, 30, 2, (Color){50, 50, 50, 175});

        return;
    }

    // RESTAR TIEMPO DEL NIVEL
    if (tiempo_restante > 0.0f)
    {
        tiempo_restante -= GetFrameTime();
    }
    float margenX = 1450.0f;
    float margenY = 50.0f;
    // float corazonSize = 40.0f; // Eliminado

    // HUD
    DrawRectangle(1400, 0, 520, 1080, color_tab);
    DrawTextEx(fuenteLilita, TextFormat("Tiempo: %.0f", tiempo_restante), (Vector2){margenX + 4.0f, margenY + 4.0f}, 50, 2, BLACK);
    DrawTextEx(fuenteLilita, TextFormat("Tiempo: %.0f", tiempo_restante), (Vector2){margenX, margenY}, 50, 2, YELLOW);

    DrawTextEx(fuenteLilita, (turno_jugador && !turno_bot) ? "TU TURNO" : "TURNO BOT", (Vector2){margenX + 4.0f, margenY + 80 + 4.0f}, 42, 2, BLACK);
    DrawTextEx(fuenteLilita, (turno_jugador && !turno_bot) ? "TU TURNO" : "TURNO BOT", (Vector2){margenX, margenY + 80}, 42, 2, (turno_jugador ? GREEN : RED));

    DrawTextEx(fuenteLilita, TextFormat("JUGADOR: %d", pares_jugador), (Vector2){margenX + 4.0f, margenY + 160 + 4.0f}, 42, 2, BLACK);
    DrawTextEx(fuenteLilita, TextFormat("JUGADOR: %d", pares_jugador), (Vector2){margenX, margenY + 160}, 42, 2, BLUE);

    DrawTextEx(fuenteLilita, TextFormat("BOT: %d", pares_bot), (Vector2){margenX + 4.0f, margenY + 240 + 4.0f}, 42, 2, BLACK);
    DrawTextEx(fuenteLilita, TextFormat("BOT: %d", pares_bot), (Vector2){margenX, margenY + 240}, 42, 2, ORANGE);

    // Mostrar mensaje de acierto temporal
    if (tiempoMensaje > 0.0f && fraseActual != NULL)
    {
        Color colorMensaje = turno_jugador ? GREEN : RED;
        DrawTextEx(fuenteLilita, fraseActual, (Vector2){margenX, margenY + 300}, 42, 2, colorMensaje);
        tiempoMensaje -= GetFrameTime();
    }

    // 1) Si hay un tiempo_espera activo: ocultar las cartas pendientes (sin preguntar por estado)
    if (tiempo_espera > 0.0f)
    {
        tiempo_espera -= GetFrameTime();
        if (tiempo_espera <= 0.0f)
        {
            // Si las cartas pendientes pertenecen al bot -> ocultarlas y pasar turno al jugador
            if (primera_bot != -1 && segunda_bot != -1)
            {
                // Solo ocultar si no fueron acertadas (estado != 2)
                if (tablero[primera_bot].estado != 2)
                    tablero[primera_bot].estado = 0;
                if (tablero[segunda_bot].estado != 2)
                    tablero[segunda_bot].estado = 0;
                primera_bot = -1;
                segunda_bot = -1;
                turno_bot = false;
                turno_jugador = true;
            }
            // Si las cartas pendientes pertenecen al jugador -> ocultarlas y pasar turno al bot
            else if (primera_carta != -1 && segunda_carta != -1)
            {
                if (tablero[primera_carta].estado != 2)
                    tablero[primera_carta].estado = 0;
                if (tablero[segunda_carta].estado != 2)
                    tablero[segunda_carta].estado = 0;
                primera_carta = -1;
                segunda_carta = -1;
                turno_jugador = false;
                turno_bot = true;
            }
        }
    }

    // 2) Lógica del bot (solo si es su turno y no hay espera de ocultado)
    if (turno_bot && !turno_jugador && tiempo_espera <= 0.0f)
    {
        const float BOT_DELAY = 0.6f; // retraso natural entre movimientos
        int totalCartas = 6;          // ajusta si tu tablero cambia

        // PRIMERA CARTA
        if (primera_bot == -1)
        {
            tiempo_accion_bot -= GetFrameTime();
            if (tiempo_accion_bot <= 0.0f)
            {
                do
                {
                    primera_bot = rand() % totalCartas;
                } while (tablero[primera_bot].estado != 0);
                tablero[primera_bot].estado = 1;
                tiempo_accion_bot = BOT_DELAY;
            }
        }
        // SEGUNDA CARTA
        else if (segunda_bot == -1)
        {
            tiempo_accion_bot -= GetFrameTime();
            if (tiempo_accion_bot <= 0.0f)
            {
                do
                {
                    segunda_bot = rand() % totalCartas;
                } while (segunda_bot == primera_bot || tablero[segunda_bot].estado != 0);
                tablero[segunda_bot].estado = 1;
                tiempo_accion_bot = BOT_DELAY;
            }
        }
        // EVALUAR PAR
        else
        {
            tiempo_accion_bot -= GetFrameTime();
            if (tiempo_accion_bot <= 0.0f)
            {
                if (tablero[primera_bot].id_par == tablero[segunda_bot].id_par)
                {
                    // ACERTO: incrementar contador solo aquí
                    tablero[primera_bot].estado = 2;
                    tablero[segunda_bot].estado = 2;
                    pares_bot++; // **aquí cuenta correctamente**
                    fraseActual = frases[rand() % 14];
                    tiempoMensaje = 1.0f;

                    // LIMPIAR INDICES DESPUES DE PROCESAR
                    PlaySound(pair);
                    primera_bot = -1;
                    segunda_bot = -1;
                    tiempo_accion_bot = BOT_DELAY;
                }
                else
                {
                    // FALLO: mostrar un momento y luego ocultar
                    tiempo_espera = BOT_DELAY;
                    // No limpiar índices, bloque tiempo_espera se encargará de ocultar y cambiar turno
                }
            }
        }
    }

    // 3) Dibujo del tablero
    dibujar_tablero(200, 200, tablero, 3, 2);
    DrawLineEx((Vector2){1400, 0}, (Vector2){1400, 1080}, 10.0f, color_carta);
    DrawTextureEx(fract, (Vector2){1450, 680}, 0.0f, 0.4, WHITE);

    // 4) Turno del jugador: manejo de clics
    if (turno_jugador && !turno_bot && tiempo_espera <= 0.0f && click && !click_procesado)
    {
        int idx = 0;
        for (int y = 0; y < 2; y++)
        {
            for (int x = 0; x < 3; x++)
            {
                Rectangle cell = {
                    (float)200 + x * 315,
                    (float)200 + y * 315,
                    300,
                    300};

                if (CheckCollisionPointRec(mouse, cell) && !click_procesado)
                {
                    if (tablero[idx].estado == 0)
                    {
                        tablero[idx].estado = 1;
                        click_procesado = true;

                        if (primera_carta == -1)
                        {
                            primera_carta = idx;
                        }
                        else if (segunda_carta == -1 && idx != primera_carta)
                        {
                            segunda_carta = idx;

                            // evaluar par del jugador
                            if (tablero[primera_carta].id_par == tablero[segunda_carta].id_par)
                            {
                                tablero[primera_carta].estado = 2;
                                tablero[segunda_carta].estado = 2;
                                // Eliminado: pares_encontrados++;
                                pares_jugador++;
                                // Eliminado: vida_rival--;
                                fraseActual = frases[rand() % 14];
                                tiempoMensaje = 1.0f;

                                // IMPORTANT: resetear indices del jugador y reiniciar datos del bot
                                PlaySound(pair);
                                primera_carta = -1;
                                segunda_carta = -1;
                                primera_bot = -1;
                                segunda_bot = -1;
                                tiempo_accion_bot = 0.0f;
                                // jugador sigue (no cambiamos turno)
                            }
                            else
                            {
                                // fallo jugador: dejar visible 0.8s, luego ocultar y pasar al bot
                                tiempo_espera = 0.8f;
                                // el bloque tiempo_espera ocultará y pondrá turno_bot = true
                            }
                        }
                    }
                }
                idx++;
            }
        }
    }
}

void nivel1()
{
    nivel_actual = NIVEL1;
    static Carta tablero[12]; // Tablero 4x3
    static int randomizado = 0;
    Vector2 mouse = GetMousePosition();
    bool click = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    bool click_procesado = false;

    // --- VARIABLES CLAVE DEL JUEGO DE PARES 1V1 (Estáticas) ---
    static int primera_carta = -1;
    static int segunda_carta = -1;
    static float tiempo_espera = 0.0f; // Tiempo para mostrar error/acierto
    static float tiempo_restante = 150.0f;
    static bool turno_jugador = true;
    static bool turno_bot = false;         // Añadida para control de turno explícito del bot
    static float tiempo_accion_bot = 0.0f; // Pausa entre acciones del bot
    static int primera_bot = -1;           // Índice de la primera carta del bot
    static int segunda_bot = -1;           // Índice de la segunda carta del bot
    static int pares_jugador = 0;
    static int pares_bot = 0;
    const int totalCartas = 12;
    const int totalPares = totalCartas / 2;
    // -----------------------------------------------------------

    static int cargado = 0;
    static Texture2D jefe1;
    static bool debe_resetear = false;
    static float tiempoMensaje = 0.0f; // Para los mensajes
    static const char frases[14][20] = {"MUY BIEN!", "EXCELENTE!", "GENIAL!", "PERFECTO!", "WOW!", "INCREIBLE!", "FANTASTICO!", "SUPER!", "IMPACTANTE!", "ASOMBROSO!", "BRILLANTE!", "SORPRENDENTE!", "LO LOGRASTE!", "MAGNIFICO!"};
    static const char *fraseActual = NULL;
    static bool juego_terminado = false;
    static int ganador = -1; // 1 = jugador, 0 = bot, 2 = empate

    // La lógica anterior de "vida_rival" e "intentos_fallidos" del jugador ha sido eliminada.

    if (debe_resetear)
    {
        randomizado = 0;
        primera_carta = -1;
        segunda_carta = -1;
        tiempo_espera = 0.0f;
        tiempo_restante = 150.0f;
        turno_jugador = true;
        turno_bot = false;
        tiempo_accion_bot = 0.0f;
        primera_bot = -1;
        segunda_bot = -1;
        pares_jugador = 0;
        pares_bot = 0;
        debe_resetear = false;
        fraseActual = NULL;
        tiempoMensaje = 0.0f;
        juego_terminado = false;
        ganador = -1;
    }

    if (cargado == 0)
    {
        jefe1 = LoadTexture("assets/images/boss1.png");
        cargado = 1;
    }

    if (!randomizado)
    {
        randomizar_nivel1(tablero, totalCartas);
        randomizado = 1;
    }

    // --- LÓGICA DE FIN DE JUEGO TEMPRANA ---
    if ((pares_jugador + pares_bot >= totalPares) || (tiempo_restante <= 0.0f))
    {
        juego_terminado = true;
    }

    if (juego_terminado)
    {
        if (pares_jugador > pares_bot)
            ganador = 1; // Jugador gana
        else if (pares_bot > pares_jugador)
            ganador = 0; // Bot gana
        else
            ganador = 2; // Empate

        // Pasamos 0 en 'vida_rival' ya que la mecánica fue eliminada.
        pantalla_final(tiempo_restante, 0, ganador, &debe_resetear);
        return; // SALIMOS de nivel1() inmediatamente
    }
    // ------------------------------------------------------------------

    // ACTUALIZAR TIEMPO
    if (tiempo_restante > 0.0f)
    {
        tiempo_restante -= GetFrameTime();
    }

    float margenX = 1450.0f;
    float margenY = 50.0f;

    // HUD
    DrawRectangle(1400, 0, 520, 1080, color_tab);
    DrawTextEx(fuenteLilita, TextFormat("Tiempo: %.0f", tiempo_restante), (Vector2){margenX + 4.0f, margenY + 4.0f}, 50, 2, BLACK);
    DrawTextEx(fuenteLilita, TextFormat("Tiempo: %.0f", tiempo_restante), (Vector2){margenX, margenY}, 50, 2, YELLOW);

    DrawTextEx(fuenteLilita, (turno_jugador && !turno_bot) ? "TU TURNO" : "TURNO BOT", (Vector2){margenX + 4.0f, margenY + 80 + 4.0f}, 42, 2, BLACK);
    DrawTextEx(fuenteLilita, (turno_jugador && !turno_bot) ? "TU TURNO" : "TURNO BOT", (Vector2){margenX, margenY + 80}, 42, 2, (turno_jugador ? GREEN : RED));

    DrawTextEx(fuenteLilita, TextFormat("JUGADOR: %d", pares_jugador), (Vector2){margenX + 4.0f, margenY + 160 + 4.0f}, 42, 2, BLACK);
    DrawTextEx(fuenteLilita, TextFormat("JUGADOR: %d", pares_jugador), (Vector2){margenX, margenY + 160}, 42, 2, BLUE);

    DrawTextEx(fuenteLilita, TextFormat("BOT: %d", pares_bot), (Vector2){margenX + 4.0f, margenY + 240 + 4.0f}, 42, 2, BLACK);
    DrawTextEx(fuenteLilita, TextFormat("BOT: %d", pares_bot), (Vector2){margenX, margenY + 240}, 42, 2, ORANGE);

    // Mostrar mensaje de acierto temporal
    if (tiempoMensaje > 0.0f && fraseActual != NULL)
    {
        Color colorMensaje = turno_jugador ? GREEN : RED;
        DrawTextEx(fuenteLilita, fraseActual, (Vector2){margenX, margenY + 300}, 42, 2, colorMensaje);
        tiempoMensaje -= GetFrameTime();
    }

    // 1) Si hay un tiempo_espera activo: ocultar las cartas y cambiar turno
    if (tiempo_espera > 0.0f)
    {
        tiempo_espera -= GetFrameTime();
        if (tiempo_espera <= 0.0f)
        {
            // Ocultar cartas del BOT
            if (primera_bot != -1 && segunda_bot != -1)
            {
                if (tablero[primera_bot].estado != 2)
                    tablero[primera_bot].estado = 0;
                if (tablero[segunda_bot].estado != 2)
                    tablero[segunda_bot].estado = 0;
                primera_bot = -1;
                segunda_bot = -1;
                turno_bot = false;
                turno_jugador = true;
            }
            // Ocultar cartas del JUGADOR
            else if (primera_carta != -1 && segunda_carta != -1)
            {
                if (tablero[primera_carta].estado != 2)
                    tablero[primera_carta].estado = 0;
                if (tablero[segunda_carta].estado != 2)
                    tablero[segunda_carta].estado = 0;
                primera_carta = -1;
                segunda_carta = -1;
                turno_jugador = false;
                turno_bot = true;
                tiempo_accion_bot = 0.0f; // Asegura que el bot empiece a actuar inmediatamente
            }
        }
    }

    // 2) Lógica del bot (solo si es su turno y no hay espera de ocultado)
    if (turno_bot && !turno_jugador && tiempo_espera <= 0.0f)
    {
        const float BOT_DELAY = 0.6f;

        // PRIMERA CARTA
        if (primera_bot == -1)
        {
            tiempo_accion_bot -= GetFrameTime();
            if (tiempo_accion_bot <= 0.0f)
            {
                do
                {
                    primera_bot = rand() % totalCartas;
                } while (tablero[primera_bot].estado != 0);
                tablero[primera_bot].estado = 1;
                tiempo_accion_bot = BOT_DELAY;
            }
        }
        // SEGUNDA CARTA
        else if (segunda_bot == -1)
        {
            tiempo_accion_bot -= GetFrameTime();
            if (tiempo_accion_bot <= 0.0f)
            {
                do
                {
                    segunda_bot = rand() % totalCartas;
                } while (segunda_bot == primera_bot || tablero[segunda_bot].estado != 0);
                tablero[segunda_bot].estado = 1;
                tiempo_accion_bot = BOT_DELAY;
            }
        }
        // EVALUAR PAR
        else
        {
            tiempo_accion_bot -= GetFrameTime();
            if (tiempo_accion_bot <= 0.0f)
            {
                if (tablero[primera_bot].id_par == tablero[segunda_bot].id_par)
                {
                    // ACERTO: el bot sigue jugando
                    tablero[primera_bot].estado = 2;
                    tablero[segunda_bot].estado = 2;
                    pares_bot++;
                    fraseActual = frases[rand() % 14];
                    tiempoMensaje = 1.0f;
                    PlaySound(pair);

                    // Limpiar índices y reiniciar pausa para que busque el siguiente par
                    primera_bot = -1;
                    segunda_bot = -1;
                    tiempo_accion_bot = BOT_DELAY; // Pausa antes de la siguiente jugada del bot
                }
                else
                {
                    // FALLO: mostrar un momento, luego ocultar y pasar turno
                    tiempo_espera = BOT_DELAY;
                    // El bloque tiempo_espera se encargará de ocultar y cambiar turno a turno_jugador = true
                }
            }
        }
    }

    // 3) Dibujo del tablero y HUD
    dibujar_tablero(200, 150, tablero, 4, 3);
    DrawLineEx((Vector2){1400, 0}, (Vector2){1400, 1080}, 10.0f, color_carta);
    DrawTextureEx(jefe1, (Vector2){1450, 680}, 0.0f, 0.4, WHITE);

    // 4) Turno del jugador: manejo de clics (solo si es su turno y no hay espera de ocultado)
    if (turno_jugador && !turno_bot && tiempo_espera <= 0.0f && click && !click_procesado)
    {
        int index = 0;
        for (int y = 0; y < 3; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                Rectangle cell = {
                    (float)200 + x * 265,
                    (float)150 + y * 265,
                    250,
                    250};

                if (CheckCollisionPointRec(mouse, cell) && !click_procesado)
                {
                    // Solo si la carta está oculta (estado 0)
                    if (tablero[index].estado == 0)
                    {
                        tablero[index].estado = 1;
                        click_procesado = true;

                        if (primera_carta == -1)
                        {
                            primera_carta = index;
                        }
                        else if (segunda_carta == -1 && index != primera_carta)
                        {
                            segunda_carta = index;

                            // evaluar par del jugador
                            if (tablero[primera_carta].id_par == tablero[segunda_carta].id_par)
                            {
                                // ACIERTO: jugador sigue jugando
                                tablero[primera_carta].estado = 2;
                                tablero[segunda_carta].estado = 2;
                                pares_jugador++;

                                fraseActual = frases[rand() % 14];
                                tiempoMensaje = 1.0f;

                                // Resetear índices para la próxima jugada del jugador
                                PlaySound(pair);
                                primera_carta = -1;
                                segunda_carta = -1;

                                // Resetear índices del bot para evitar conflictos
                                primera_bot = -1;
                                segunda_bot = -1;
                                tiempo_accion_bot = 0.0f;
                                // turno_jugador = true (ya lo es)
                            }
                            else
                            {
                                // FALLO: dejar visible 0.8s, luego ocultar y pasar al bot
                                tiempo_espera = 0.8f;
                                // El bloque tiempo_espera se encargará de ocultar y poner turno_bot = true
                            }
                        }
                    }
                }
                index++;
            }
        }
    }
}

void nivel2()
{
    nivel_actual = NIVEL2;
    static Carta tablero[16]; // Tablero 4x4 (16 cartas / 8 pares)
    static int randomizado = 0;
    Vector2 mouse = GetMousePosition();
    bool click = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    bool click_procesado = false;

    // --- VARIABLES CLAVE DEL JUEGO DE PARES 1V1 (Estáticas) ---
    static int primera_carta = -1;
    static int segunda_carta = -1;
    static float tiempo_espera = 0.0f; // Tiempo para mostrar error/acierto
    static float tiempo_restante = 180.0f;
    static bool turno_jugador = true;
    static bool turno_bot = false;
    static float tiempo_accion_bot = 0.0f; // Pausa entre acciones del bot
    static int primera_bot = -1;
    static int segunda_bot = -1;
    static int pares_jugador = 0; // Sustituye a pares_encontrados
    static int pares_bot = 0;     // Sustituye a vida_rival
    const int totalCartas = 16;
    const int totalPares = totalCartas / 2;
    // -----------------------------------------------------------

    static int cargado = 0;
    static Texture2D jefe2;
    static bool debe_resetear = false;
    static float tiempoMensaje = 0.0f;
    static const char frases[14][20] = {"MUY BIEN!", "EXCELENTE!", "GENIAL!", "PERFECTO!", "WOW!", "INCREIBLE!", "FANTASTICO!", "SUPER!", "IMPACTANTE!", "ASOMBROSO!", "BRILLANTE!", "SORPRENDENTE!", "LO LOGRASTE!", "MAGNIFICO!"};
    static const char *fraseActual = NULL;
    static bool juego_terminado = false;
    static int ganador = -1; // 1 = jugador, 0 = bot, 2 = empate

    // La lógica anterior de "vida_rival" e "intentos_fallidos" ha sido eliminada.

    if (debe_resetear)
    {
        randomizado = 0;
        primera_carta = -1;
        segunda_carta = -1;
        tiempo_espera = 0.0f;
        pares_jugador = 0;
        pares_bot = 0;
        tiempo_restante = 180.0f;
        turno_jugador = true;
        turno_bot = false;
        tiempo_accion_bot = 0.0f;
        primera_bot = -1;
        segunda_bot = -1;
        debe_resetear = false;
        fraseActual = NULL;
        tiempoMensaje = 0.0f;
        juego_terminado = false;
        ganador = -1;
    }

    if (cargado == 0)
    {
        jefe2 = LoadTexture("assets/images/boss2.png");
        cargado = 1;
    }

    if (!randomizado)
    {
        randomizar_nivel2(tablero, totalCartas);
        randomizado = 1;
    }

    // --- LÓGICA DE FIN DE JUEGO TEMPRANA ---
    if ((pares_jugador + pares_bot >= totalPares) || (tiempo_restante <= 0.0f))
    {
        juego_terminado = true;
    }

    if (juego_terminado)
    {
        if (pares_jugador > pares_bot)
            ganador = 1; // Jugador gana
        else if (pares_bot > pares_jugador)
            ganador = 0; // Bot gana
        else
            ganador = 2; // Empate

        // Pasamos 0 en 'vida_rival' (ignorada) y el 'ganador' (2 = nivel 2 en tu vieja lógica,
        // pero ahora significa EMPATE, ¡cuidado!)
        // El parámetro 'nivel' de pantalla_final ahora es 'ganador'.
        pantalla_final(tiempo_restante, 0, ganador, &debe_resetear);
        return; // SALIMOS de nivel2() inmediatamente
    }
    // ------------------------------------------------------------------

    // ACTUALIZAR TIEMPO
    if (tiempo_restante > 0.0f)
    {
        tiempo_restante -= GetFrameTime();
    }

    float margenX = 1450.0f;
    float margenY = 50.0f;

    // HUD
    DrawRectangle(1400, 0, 520, 1080, color_tab);
    DrawTextEx(fuenteLilita, TextFormat("Tiempo: %.0f", tiempo_restante), (Vector2){margenX + 4.0f, margenY + 4.0f}, 50, 2, BLACK);
    DrawTextEx(fuenteLilita, TextFormat("Tiempo: %.0f", tiempo_restante), (Vector2){margenX, margenY}, 50, 2, YELLOW);

    DrawTextEx(fuenteLilita, (turno_jugador && !turno_bot) ? "TU TURNO" : "TURNO BOT", (Vector2){margenX + 4.0f, margenY + 80 + 4.0f}, 42, 2, BLACK);
    DrawTextEx(fuenteLilita, (turno_jugador && !turno_bot) ? "TU TURNO" : "TURNO BOT", (Vector2){margenX, margenY + 80}, 42, 2, (turno_jugador ? GREEN : RED));

    DrawTextEx(fuenteLilita, TextFormat("JUGADOR: %d", pares_jugador), (Vector2){margenX + 4.0f, margenY + 160 + 4.0f}, 42, 2, BLACK);
    DrawTextEx(fuenteLilita, TextFormat("JUGADOR: %d", pares_jugador), (Vector2){margenX, margenY + 160}, 42, 2, BLUE);

    DrawTextEx(fuenteLilita, TextFormat("BOT: %d", pares_bot), (Vector2){margenX + 4.0f, margenY + 240 + 4.0f}, 42, 2, BLACK);
    DrawTextEx(fuenteLilita, TextFormat("BOT: %d", pares_bot), (Vector2){margenX, margenY + 240}, 42, 2, ORANGE);

    // Mostrar mensaje de acierto temporal
    if (tiempoMensaje > 0.0f && fraseActual != NULL)
    {
        Color colorMensaje = turno_jugador ? GREEN : RED;
        DrawTextEx(fuenteLilita, fraseActual, (Vector2){margenX, margenY + 300}, 42, 2, colorMensaje);
        tiempoMensaje -= GetFrameTime();
    }

    // 1) Si hay un tiempo_espera activo: ocultar las cartas y cambiar turno
    if (tiempo_espera > 0.0f)
    {
        tiempo_espera -= GetFrameTime();
        if (tiempo_espera <= 0.0f)
        {
            // Ocultar cartas del BOT
            if (primera_bot != -1 && segunda_bot != -1)
            {
                if (tablero[primera_bot].estado != 2)
                    tablero[primera_bot].estado = 0;
                if (tablero[segunda_bot].estado != 2)
                    tablero[segunda_bot].estado = 0;
                primera_bot = -1;
                segunda_bot = -1;
                turno_bot = false;
                turno_jugador = true;
            }
            // Ocultar cartas del JUGADOR
            else if (primera_carta != -1 && segunda_carta != -1)
            {
                if (tablero[primera_carta].estado != 2)
                    tablero[primera_carta].estado = 0;
                if (tablero[segunda_carta].estado != 2)
                    tablero[segunda_carta].estado = 0;
                primera_carta = -1;
                segunda_carta = -1;
                turno_jugador = false;
                turno_bot = true;
                tiempo_accion_bot = 0.0f;
            }
        }
    }

    // 2) Lógica del bot (turno_bot)
    if (turno_bot && !turno_jugador && tiempo_espera <= 0.0f)
    {
        const float BOT_DELAY = 0.5f; // Bot un poco más rápido que en nivel 1?

        // PRIMERA CARTA
        if (primera_bot == -1)
        {
            tiempo_accion_bot -= GetFrameTime();
            if (tiempo_accion_bot <= 0.0f)
            {
                do
                {
                    primera_bot = rand() % totalCartas;
                } while (tablero[primera_bot].estado != 0);
                tablero[primera_bot].estado = 1;
                tiempo_accion_bot = BOT_DELAY;
            }
        }
        // SEGUNDA CARTA
        else if (segunda_bot == -1)
        {
            tiempo_accion_bot -= GetFrameTime();
            if (tiempo_accion_bot <= 0.0f)
            {
                do
                {
                    segunda_bot = rand() % totalCartas;
                } while (segunda_bot == primera_bot || tablero[segunda_bot].estado != 0);
                tablero[segunda_bot].estado = 1;
                tiempo_accion_bot = BOT_DELAY;
            }
        }
        // EVALUAR PAR
        else
        {
            tiempo_accion_bot -= GetFrameTime();
            if (tiempo_accion_bot <= 0.0f)
            {
                if (tablero[primera_bot].id_par == tablero[segunda_bot].id_par)
                {
                    // ACIERTO: el bot sigue jugando
                    tablero[primera_bot].estado = 2;
                    tablero[segunda_bot].estado = 2;
                    pares_bot++;
                    fraseActual = frases[rand() % 14];
                    tiempoMensaje = 1.0f;
                    PlaySound(pair);

                    primera_bot = -1;
                    segunda_bot = -1;
                    tiempo_accion_bot = BOT_DELAY;
                }
                else
                {
                    // FALLO: mostrar un momento, luego ocultar y pasar turno
                    tiempo_espera = BOT_DELAY;
                }
            }
        }
    }

    // 3) Dibujo del tablero y jefe
    // El tablero es 4x4, necesitamos ajustar el dibujar_tablero (asumo que la función
    // puede manejar esto con los parámetros 4, 4 y el tamaño de las cartas/espacio)
    dibujar_tablero(200, 100, tablero, 4, 4);
    DrawLineEx((Vector2){1400, 0}, (Vector2){1400, 1080}, 10.0f, color_carta);
    DrawTextureEx(jefe2, (Vector2){1450, 680}, 0.0f, 0.4, WHITE);

    // 4) Turno del jugador: manejo de clics (solo si es su turno y no hay espera)
    if (turno_jugador && !turno_bot && tiempo_espera <= 0.0f && click && !click_procesado)
    {
        int index = 0;
        for (int y = 0; y < 4; y++) // 4 filas
        {
            for (int x = 0; x < 4; x++) // 4 columnas
            {
                Rectangle cell = {
                    (float)200 + x * 235,
                    (float)100 + y * 235,
                    220,
                    220};

                if (CheckCollisionPointRec(mouse, cell) && !click_procesado)
                {
                    if (tablero[index].estado == 0) // Solo si está oculta
                    {
                        tablero[index].estado = 1;
                        click_procesado = true;

                        if (primera_carta == -1)
                        {
                            primera_carta = index;
                        }
                        else if (segunda_carta == -1 && index != primera_carta)
                        {
                            segunda_carta = index;

                            // evaluar par del jugador
                            if (tablero[primera_carta].id_par == tablero[segunda_carta].id_par)
                            {
                                // ACIERTO: jugador sigue jugando
                                tablero[primera_carta].estado = 2;
                                tablero[segunda_carta].estado = 2;
                                pares_jugador++; // Incrementa pares del jugador

                                fraseActual = frases[rand() % 14];
                                tiempoMensaje = 1.0f;
                                PlaySound(pair);
                                primera_carta = -1;
                                segunda_carta = -1;

                                primera_bot = -1;
                                segunda_bot = -1;
                                tiempo_accion_bot = 0.0f;
                            }
                            else
                            {
                                // FALLO: pasar turno al bot
                                tiempo_espera = 0.8f;
                            }
                        }
                    }
                }
                index++;
            }
        }
    }
}

void nivel3()
{
    nivel_actual = NIVEL3;
    static Carta tablero[20]; // Tablero 5x4 (20 cartas / 10 pares)
    static int randomizado = 0;
    Vector2 mouse = GetMousePosition();
    bool click = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    bool click_procesado = false;

    // --- VARIABLES CLAVE DEL JUEGO DE PARES 1V1 (Estáticas) ---
    static int primera_carta = -1;
    static int segunda_carta = -1;
    static float tiempo_espera = 0.0f; // Tiempo para mostrar error/acierto
    static float tiempo_restante = 210.0f;
    static bool turno_jugador = true;
    static bool turno_bot = false;
    static float tiempo_accion_bot = 0.0f; // Pausa entre acciones del bot
    static int primera_bot = -1;
    static int segunda_bot = -1;
    static int pares_jugador = 0; // Sustituye a pares_encontrados
    static int pares_bot = 0;     // Sustituye a vida_rival
    const int totalCartas = 20;
    const int totalPares = totalCartas / 2;
    // -----------------------------------------------------------

    static int cargado = 0;
    static Texture2D jefe3;
    static bool debe_resetear = false;
    static float tiempoMensaje = 0.0f;
    static const char frases[14][20] = {"MUY BIEN!", "EXCELENTE!", "GENIAL!", "PERFECTO!", "WOW!", "INCREIBLE!", "FANTASTICO!", "SUPER!", "IMPACTANTE!", "ASOMBROSO!", "BRILLANTE!", "SORPRENDENTE!", "LO LOGRASTE!", "MAGNIFICO!"};
    static const char *fraseActual = NULL;
    static bool juego_terminado = false;
    static int ganador = -1; // 1 = jugador, 0 = bot, 2 = empate

    // La lógica anterior de "vida_rival" e "intentos_fallidos" ha sido eliminada.

    if (debe_resetear)
    {
        randomizado = 0;
        primera_carta = -1;
        segunda_carta = -1;
        tiempo_espera = 0.0f;
        pares_jugador = 0;
        pares_bot = 0;
        tiempo_restante = 210.0f;
        turno_jugador = true;
        turno_bot = false;
        tiempo_accion_bot = 0.0f;
        primera_bot = -1;
        segunda_bot = -1;
        debe_resetear = false;
        fraseActual = NULL;
        tiempoMensaje = 0.0f;
        juego_terminado = false;
        ganador = -1;
    }

    if (cargado == 0)
    {
        jefe3 = LoadTexture("assets/images/boss3.png");
        cargado = 1;
    }

    if (!randomizado)
    {
        randomizar_nivel3(tablero, totalCartas);
        randomizado = 1;
    }

    // --- LÓGICA DE FIN DE JUEGO TEMPRANA ---
    if ((pares_jugador + pares_bot >= totalPares) || (tiempo_restante <= 0.0f))
    {
        juego_terminado = true;
    }

    if (juego_terminado)
    {
        if (pares_jugador > pares_bot)
            ganador = 1; // Jugador gana
        else if (pares_bot > pares_jugador)
            ganador = 0; // Bot gana
        else
            ganador = 2; // Empate

        // Pasamos 0 en 'vida_rival' y el 'ganador'.
        pantalla_final(tiempo_restante, 0, ganador, &debe_resetear);
        return;
    }
    // ------------------------------------------------------------------

    // ACTUALIZAR TIEMPO
    if (tiempo_restante > 0.0f)
    {
        tiempo_restante -= GetFrameTime();
    }

    float margenX = 1450.0f;
    float margenY = 50.0f;

    // HUD
    DrawRectangle(1400, 0, 520, 1080, color_tab);
    DrawTextEx(fuenteLilita, TextFormat("Tiempo: %.0f", tiempo_restante), (Vector2){margenX + 4.0f, margenY + 4.0f}, 50, 2, BLACK);
    DrawTextEx(fuenteLilita, TextFormat("Tiempo: %.0f", tiempo_restante), (Vector2){margenX, margenY}, 50, 2, YELLOW);

    DrawTextEx(fuenteLilita, (turno_jugador && !turno_bot) ? "TU TURNO" : "TURNO BOT", (Vector2){margenX + 4.0f, margenY + 80 + 4.0f}, 42, 2, BLACK);
    DrawTextEx(fuenteLilita, (turno_jugador && !turno_bot) ? "TU TURNO" : "TURNO BOT", (Vector2){margenX, margenY + 80}, 42, 2, (turno_jugador ? GREEN : RED));

    DrawTextEx(fuenteLilita, TextFormat("JUGADOR: %d", pares_jugador), (Vector2){margenX + 4.0f, margenY + 160 + 4.0f}, 42, 2, BLACK);
    DrawTextEx(fuenteLilita, TextFormat("JUGADOR: %d", pares_jugador), (Vector2){margenX, margenY + 160}, 42, 2, BLUE);

    DrawTextEx(fuenteLilita, TextFormat("BOT: %d", pares_bot), (Vector2){margenX + 4.0f, margenY + 240 + 4.0f}, 42, 2, BLACK);
    DrawTextEx(fuenteLilita, TextFormat("BOT: %d", pares_bot), (Vector2){margenX, margenY + 240}, 42, 2, ORANGE);

    // Mostrar mensaje de acierto temporal
    if (tiempoMensaje > 0.0f && fraseActual != NULL)
    {
        Color colorMensaje = turno_jugador ? GREEN : RED;
        DrawTextEx(fuenteLilita, fraseActual, (Vector2){margenX, margenY + 300}, 42, 2, colorMensaje);
        tiempoMensaje -= GetFrameTime();
    }

    // 1) Si hay un tiempo_espera activo: ocultar las cartas y cambiar turno
    if (tiempo_espera > 0.0f)
    {
        tiempo_espera -= GetFrameTime();
        if (tiempo_espera <= 0.0f)
        {
            // Ocultar cartas del BOT
            if (primera_bot != -1 && segunda_bot != -1)
            {
                if (tablero[primera_bot].estado != 2)
                    tablero[primera_bot].estado = 0;
                if (tablero[segunda_bot].estado != 2)
                    tablero[segunda_bot].estado = 0;
                primera_bot = -1;
                segunda_bot = -1;
                turno_bot = false;
                turno_jugador = true;
            }
            // Ocultar cartas del JUGADOR
            else if (primera_carta != -1 && segunda_carta != -1)
            {
                if (tablero[primera_carta].estado != 2)
                    tablero[primera_carta].estado = 0;
                if (tablero[segunda_carta].estado != 2)
                    tablero[segunda_carta].estado = 0;
                primera_carta = -1;
                segunda_carta = -1;
                turno_jugador = false;
                turno_bot = true;
                tiempo_accion_bot = 0.0f;
            }
        }
    }

    // 2) Lógica del bot (turno_bot)
    if (turno_bot && !turno_jugador && tiempo_espera <= 0.0f)
    {
        const float BOT_DELAY = 0.4f; // Bot aún más rápido

        // PRIMERA CARTA
        if (primera_bot == -1)
        {
            tiempo_accion_bot -= GetFrameTime();
            if (tiempo_accion_bot <= 0.0f)
            {
                do
                {
                    primera_bot = rand() % totalCartas;
                } while (tablero[primera_bot].estado != 0);
                tablero[primera_bot].estado = 1;
                tiempo_accion_bot = BOT_DELAY;
            }
        }
        // SEGUNDA CARTA
        else if (segunda_bot == -1)
        {
            tiempo_accion_bot -= GetFrameTime();
            if (tiempo_accion_bot <= 0.0f)
            {
                do
                {
                    segunda_bot = rand() % totalCartas;
                } while (segunda_bot == primera_bot || tablero[segunda_bot].estado != 0);
                tablero[segunda_bot].estado = 1;
                tiempo_accion_bot = BOT_DELAY;
            }
        }
        // EVALUAR PAR
        else
        {
            tiempo_accion_bot -= GetFrameTime();
            if (tiempo_accion_bot <= 0.0f)
            {
                if (tablero[primera_bot].id_par == tablero[segunda_bot].id_par)
                {
                    // ACIERTO: el bot sigue jugando
                    tablero[primera_bot].estado = 2;
                    tablero[segunda_bot].estado = 2;
                    pares_bot++;
                    fraseActual = frases[rand() % 14];
                    tiempoMensaje = 1.0f;
                    PlaySound(pair);
                    primera_bot = -1;
                    segunda_bot = -1;
                    tiempo_accion_bot = BOT_DELAY;
                }
                else
                {
                    // FALLO: mostrar un momento, luego ocultar y pasar turno
                    tiempo_espera = BOT_DELAY;
                }
            }
        }
    }

    // 3) Dibujo del tablero y jefe
    // El tablero es 5x4, necesitamos ajustar el dibujar_tablero.
    // Lo llamaremos con (180, 100, tablero, 5, 4) como estaba en tu código original.
    dibujar_tablero(180, 100, tablero, 5, 4);
    DrawLineEx((Vector2){1400, 0}, (Vector2){1400, 1080}, 10.0f, color_carta);
    DrawTextureEx(jefe3, (Vector2){1450, 680}, 0.0f, 0.4, WHITE);

    // 4) Turno del jugador: manejo de clics (solo si es su turno y no hay espera)
    if (turno_jugador && !turno_bot && tiempo_espera <= 0.0f && click && !click_procesado)
    {
        int index = 0;
        for (int y = 0; y < 4; y++) // 4 filas
        {
            for (int x = 0; x < 5; x++) // 5 columnas
            {
                Rectangle cell = {
                    (float)180 + x * 215,
                    (float)100 + y * 215,
                    200,
                    200};

                if (CheckCollisionPointRec(mouse, cell) && !click_procesado)
                {
                    if (tablero[index].estado == 0) // Solo si está oculta
                    {
                        tablero[index].estado = 1;
                        click_procesado = true;

                        if (primera_carta == -1)
                        {
                            primera_carta = index;
                        }
                        else if (segunda_carta == -1 && index != primera_carta)
                        {
                            segunda_carta = index;

                            // evaluar par del jugador
                            if (tablero[primera_carta].id_par == tablero[segunda_carta].id_par)
                            {
                                // ACIERTO: jugador sigue jugando
                                tablero[primera_carta].estado = 2;
                                tablero[segunda_carta].estado = 2;
                                pares_jugador++;

                                fraseActual = frases[rand() % 14];
                                tiempoMensaje = 1.0f;
                                PlaySound(pair);
                                primera_carta = -1;
                                segunda_carta = -1;

                                primera_bot = -1;
                                segunda_bot = -1;
                                tiempo_accion_bot = 0.0f;
                            }
                            else
                            {
                                // FALLO: pasar turno al bot
                                tiempo_espera = 0.8f;
                            }
                        }
                    }
                }
                index++;
            }
        }
    }
}

void pantalla_final(int tiempo_restante, int vida_rival, int ganador, bool *resetear)
{
    Vector2 mouse = GetMousePosition();

    Rectangle btnReintentar = {760, 660, 400, 60};
    Rectangle btnSalir = {760, 760, 400, 60};

    DrawRectangle(0, 0, 1920, 1080, (Color){0, 0, 0, 200});

    const char *titulo = "FIN DEL JUEGO";
    const char *mensajePrincipal = "";
    Color colorResultado = WHITE;
    bool esVictoria = (ganador == 1);

    if (ganador == 1) // JUGADOR GANA
    {
        mensajePrincipal = "VICTORIA! GANASTE POR PARES";
        colorResultado = GREEN;
    }
    else if (ganador == 0) // BOT GANA
    {
        if (tiempo_restante <= 0)
            mensajePrincipal = "DERROTA! SE ACABO EL TIEMPO";
        else
            mensajePrincipal = "DERROTA! EL BOT TIENE MAS PARES";

        colorResultado = RED;
    }
    else // ganador == 2 (EMPATE)
    {
        mensajePrincipal = "EMPATE! NADIE HA GANADO";
        colorResultado = YELLOW;
    }

    // --- DIBUJO DE MENSAJES PRINCIPALES ---
    DrawTextEx(fuenteLilita, titulo, (Vector2){960 - MeasureTextEx(fuenteLilita, titulo, 50, 2).x / 2, 430}, 50, 2, WHITE);
    DrawTextEx(fuenteLilita, mensajePrincipal, (Vector2){960 - MeasureTextEx(fuenteLilita, mensajePrincipal, 50, 2).x / 2, 500}, 50, 2, colorResultado);

    // --- DIBUJO DE BOTONES ---
    const char *textoBtn1 = (esVictoria && nivel_actual != NIVEL3) ? "SIGUIENTE NIVEL" : "REINTENTAR";
    DrawRectangleRec(btnReintentar, GRAY);
    DrawTextEx(fuenteLilita, textoBtn1, (Vector2){btnReintentar.x + btnReintentar.width / 2 - MeasureTextEx(fuenteLilita, textoBtn1, 40, 2).x / 2, btnReintentar.y + 10}, 40, 2, BLACK);

    DrawRectangleRec(btnSalir, GRAY);
    DrawTextEx(fuenteLilita, "SALIR", (Vector2){btnSalir.x + btnSalir.width / 2 - MeasureTextEx(fuenteLilita, "SALIR", 40, 2).x / 2, btnSalir.y + 10}, 40, 2, BLACK);

    // --- LÓGICA DE CLICKS ---
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec(mouse, btnReintentar))
        {
            *resetear = true; // Siempre reseteamos la lógica del juego anterior

            if (esVictoria && nivel_actual == TUTORIAL)
            {
                Pantalla_Actual = NIVEL1;
                return; // Evita doble clic
            }
            else if (esVictoria && nivel_actual == NIVEL1)
            {
                Pantalla_Actual = NIVEL2;
                return; // Evita doble clic
            }
            else if (esVictoria && nivel_actual == NIVEL2)
            {
                Pantalla_Actual = NIVEL3;
                return; // Evita doble clic
            }
            else if (esVictoria && nivel_actual == NIVEL3)
            {
                // Ganaste el juego completo: no hacemos nada, solo ofrecemos salir o reintentar el último nivel
            }
            else
            {
                // Derrota, Empate, o reintentar el nivel actual
                Pantalla_Actual = nivel_actual;
                return; // Evita doble clic
            }
        }

        if (CheckCollisionPointRec(mouse, btnSalir))
        {
            *resetear = true;
            Pantalla_Actual = INICIO;
            return; // Evita doble clic
        }
    }
}

void estrellas(float dt, int screenWidth, int screenHeight)
{
    if (!estrellas_inicializadas)
    {
        for (int i = 0; i < STAR_COUNT; i++)
        {
            starX[i] = (float)GetRandomValue(0, screenWidth);
            starY[i] = (float)GetRandomValue(0, screenHeight);
            starSpeed[i] = (float)GetRandomValue(40, 200) / 10.0f;
        }
        estrellas_inicializadas = 1;
    }

    for (int i = 0; i < STAR_COUNT; i++)
    {
        starY[i] += starSpeed[i] * dt;

        if (starY[i] > screenHeight)
        {
            starY[i] = 0;
            starX[i] = (float)GetRandomValue(0, screenWidth);
        }

        DrawPixel((int)starX[i], (int)starY[i], WHITE);
    }
}