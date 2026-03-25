# tv-key

**Description**: Send virtual key events to Samsung TV. Use this to navigate menus, press buttons, control volume, change channels, and interact with any on-screen UI via simulated remote control keys.  
**Category**: Device Control

## Usage

```
tv-key <keycode> [--repeat N] [--delay MS]
```

## Options

| Option | Description | Default |
|--------|-------------|---------|
| `<keycode>` | Linux input event keycode (required) | — |
| `--repeat <N>` | Number of times to send the key | `1` |
| `--delay <MS>` | Delay between repeated keys in milliseconds | `100` |

## Key Reference (Samsung TV — KantSU2e)

### Navigation

| Key | Code | Key | Code |
|-----|------|-----|------|
| Up | `111` | Down | `116` |
| Left | `113` | Right | `114` |
| Return/OK | `36` | Back (XF86Back) | `9` |
| Prior (Page Up) | `112` | Next (Page Down) | `117` |
| End | `115` | | |

### System

| Key | Code | Key | Code |
|-----|------|-----|------|
| SysMenu | `133` | SimpleMenu | `135` |
| ChannelGuide | `138` | Exit (XF86Exit) | `182` |
| Info | `196` | | |
| PowerOff (XF86PowerOff) | `124` | LFDPowerOff | `120` |

### Picture & Sound

| Key | Code | Key | Code |
|-----|------|-----|------|
| PictureSize | `140` | PictureMode | `141` |
| SoundMode | `197` | | |

### Number Pad

| Key | Code | Key | Code |
|-----|------|-----|------|
| 0 | `19` | 1 | `10` |
| 2 | `11` | 3 | `12` |
| 4 | `13` | 5 | `14` |
| 6 | `15` | 7 | `16` |
| 8 | `17` | 9 | `18` |

## Examples

```bash
# Navigate down 3 times
tv-key 116 --repeat 3

# Navigate down slowly (500ms between presses)
tv-key 116 --repeat 3 --delay 500

# Go back
tv-key 9

# Open system menu
tv-key 133

# Change picture mode
tv-key 141
```

## Output

```json
{"success": true, "keycode": 108, "repeat": 3, "delay_ms": 100}
```

## Common Workflows

Navigate to a menu item:
1. `tv-key 116 --repeat 3` (move down 3 items)
2. Use `tv-screenshot` to verify position

Go back:
1. `tv-key 9` (Back/XF86Back)

## Important

Key codes may vary by TV model. Verify with actual device before creating automation workflows. Use `tv-screenshot` to confirm navigation results.
