"""Stage platform video runtimes and their notices into a release payload."""
import argparse
from pathlib import Path
from update_video_dependencies import payload


def stage(platform, destination):
    for name, data in payload(platform).items():
        if name.startswith("video-licenses/"):
            if platform == "osx":
                name = "Resources/" + name
            elif platform.startswith("linux-"):
                name = "share/doc/Chataigne/" + name
        path = destination / name
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(data)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("platform", choices=["win-x64", "win7-x64", "osx", "linux-x64", "linux-arm64", "linux-armhf"])
    parser.add_argument("destination", type=Path)
    args = parser.parse_args()
    stage(args.platform, args.destination)
