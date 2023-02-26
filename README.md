# GlslTest

## Usage

### Files

Directory structure 
```
shaders
└── test
    ├── background // shader dir
    │   ├── frag.glsl // shader loaded from shader.json
    │   ├── shader.json // shader config
    │   └── vert.glsl // shader loaded from shader.json
    └── mask // shader dir
        ├── frag.glsl // shader loaded from shader.json
        ├── lenna.png // image loaded from shader.json
        ├── mask.png // image loaded from shader.json
        └── shader.json // shader config
```

shader.json
```json5
{
  "width": 1024,
  "height": 1024,
  "vertex": "../background/vert.glsl",
  "fragment": "frag.glsl",
  "input_data": {
    "vertices": [
      -1, -1,
      -1, 1,
      1, 1,
      1, -1
    ],
    "elements": [
      0, 1, 2,
      2, 3, 0
    ]
  },
  "input_locations": [
    {
      "name": "aPos",
      "size": 2
    }
  ],
  "uniform": {
    "uMask": ["mask.png"], // path to image to load
    "uBackground": ["../background"], // path to shader to load as texture
    "uTexture": ["lenna.png"] // path to image to load
  }
}
```

### CLI
```bash
glsltest shaders/test/mask
```
