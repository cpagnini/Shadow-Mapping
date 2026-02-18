# Shadow Mapping - OpenGL

Progetto di Computer Graphics per la realizzazione di ombre in tempo reale tramite **Shadow Mapping**.

## Tecnologie

- **OpenGL 3.3** (Core Profile)
- **GLFW** - Window management
- **GLAD** - OpenGL loader
- **GLM** - Math library

## Algoritmo

Il progetto implementa la tecnica del **Shadow Mapping** in due passaggi:

1. **Shadow Pass**: Render della scena dalla prospettiva della luce, salvando solo i valori di profondità in una depth texture (FBO)
2. **Main Pass**: Render della scena dalla camera, confrontando ogni fragment con la depth map per determinare se è in ombra

### Tecniche implementate

| Sezione Schermo | Tecnica |
|-----------------|---------|
| Sinistra | Depth Map visualization |
| Centro | Shadow senza bias (shadow acne evidente) |
| Destra | Shadow con **bias dinamico + PCF** (anti-aliasing ombre) |

## Controlli

| Tasto | Azione |
|-------|--------|
| W/A/S/D | Movimento camera |
| Mouse | Rotazione vista |
| Scroll | Zoom |
| ESC | Chiudi |

## Struttura

```
├── Shadow Mapping.cpp    # Entry point
├── ShadowMap.*           # Gestione FBO e light matrix
├── Shader.*              # Compilazione shader
├── Camera.*              # Camera FPS
├── Sphere.*              # Generazione procedurale sfere
├── Plane.*               # Piano pavimento
├── Texture.*             # Caricamento texture (stb_image)
└── shaders/
    ├── shadow.*          # Shader per shadow pass
    └── basic.*           # Shader principale con Blinn-Phong
```

## Build

Richiede Visual Studio 2022 con carico di lavoro C++. Le dipendenze sono già incluse in `Dependencis/`.

## Screenshot

La scena mostra 3 sfere di diversa dimensione su un piano, con una texture marmo applicata alla sfera centrale rotante.
