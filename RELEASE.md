# Release Notes Template

## Version v{VERSION} - {DATE}

### New Features
- New feature description 1
- New feature description 2

### Improvements
- Improvement description 1
- Improvement description 2

### Fixes
- Fixed issue 1
- Fixed issue 2

### Known Issues
- Known issue 1
- Known issue 2

### Installation Instructions

#### Build from Source
```bash
git clone https://github.com/{USERNAME}/pnana.git
cd pnana
git checkout v{VERSION}
chmod +x ./build.sh
./build.sh --install
```

#### Using Pre-compiled Packages
##### Ubuntu/Debian (.deb)
```bash
wget https://github.com/{USERNAME}/pnana/releases/download/v{VERSION}/pnana_v{VERSION}_amd64.deb
sudo dpkg -i pnana_v{VERSION}_amd64.deb
sudo apt-get install -f  # If there are dependency issues
```

##### Generic Linux (.tar.gz)
```bash
wget https://github.com/{USERNAME}/pnana/releases/download/v{VERSION}/pnana-ubuntu22.04.tar.gz
tar -xzf pnana-ubuntu22.04.tar.gz
cd package
sudo ./install.sh
```

### Usage
```bash
# Start blank editor
pnana

# Open file
pnana filename.txt

# View help
pnana --help

# View version
pnana --version
```

### Configuration
The configuration file is located at `~/.config/pnana/config.json`, where you can modify themes, shortcuts, and other settings as needed.

### Themes
pnana provides multiple built-in themes that you can switch using the following commands:
```bash
pnana --theme=dark  # Dark theme
pnana --theme=light # Light theme
```

### Plugins
pnana supports Lua plugin extensions, with the plugin directory located at `~/.config/pnana/plugins/`.

### Feedback and Support
- Bug reports: https://github.com/{USERNAME}/pnana/issues
- Feature requests: https://github.com/{USERNAME}/pnana/issues/new?template=feature_request.md
- Discussions: https://github.com/{USERNAME}/pnana/discussions

### Contributing
Code contributions are welcome! Please check [CONTRIBUTING.md](https://github.com/{USERNAME}/pnana/blob/master/CONTRIBUTING.md) for details.

### License
This project uses the [LICENSE](https://github.com/{USERNAME}/pnana/blob/master/LICENSE) license.

---

### Version History
- [v{PREV_VERSION}]({PREV_RELEASE_URL}) - {PREV_DATE}