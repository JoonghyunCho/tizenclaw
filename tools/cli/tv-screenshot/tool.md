# tv-screenshot

**Description**: Capture Samsung TV screen using Enlightenment compositor. Use this tool to see what's currently displayed on the TV screen.  
**Category**: Perception

## Usage

```
tv-screenshot [--path DIR] [--name FILE]
```

## Options

| Option | Description | Default |
|--------|-------------|---------|
| `--path <DIR>` | Output directory | `/tmp` |
| `--name <FILE>` | Output filename | `dump_screen.png` |

## Examples

```bash
# Default: saves to /tmp/dump_screen.png
tv-screenshot

# Custom path and filename
tv-screenshot --path /opt/usr/media --name screen_capture.png
```

## Output

```json
{"success": true, "path": "/tmp/dump_screen.png", "size": 245760}
```

## Integration

Combine with `tv-key` for screen-aware TV automation:
1. Capture screen with `tv-screenshot`
2. Analyze the captured image
3. Send appropriate key events with `tv-key`
