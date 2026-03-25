# tv-screenshot

**Description**: Capture Samsung TV screen using Enlightenment compositor. Use this tool to see what's currently displayed on the TV screen. The captured image is automatically analyzed by the AI for screen-aware automation.  
**Category**: Perception

## Usage

```
tv-screenshot [--path DIR] [--name NAME] [--quality Q]
```

## Options

| Option | Description | Default |
|--------|-------------|---------|
| `--path <DIR>` | Output directory | `/tmp` |
| `--name <NAME>` | Base filename (without extension) | `tizenclaw_screen` |
| `--quality <Q>` | JPEG quality (1-100, lower = smaller) | `50` |

## Behavior

- **Old file cleanup**: Automatically deletes the previous screenshot before capture
- **Size optimization**: Resizes to 960×540 and converts to JPEG (~50-100KB instead of 2-4MB PNG)
- **Fallback**: If JPEG conversion is unavailable, outputs original PNG

## Examples

```bash
# Default: captures to /tmp/tizenclaw_screen.jpg
tv-screenshot

# Custom quality (higher = better quality, larger file)
tv-screenshot --quality 70
```

## Output

```json
{"success": true, "path": "/tmp/tizenclaw_screen.jpg", "size": 65432}
```

## Integration

Combine with `tv-key` for screen-aware TV automation:
1. Capture screen with `tv-screenshot`
2. AI automatically analyzes the captured image
3. Send appropriate key events with `tv-key`
4. Repeat until the desired screen state is reached
