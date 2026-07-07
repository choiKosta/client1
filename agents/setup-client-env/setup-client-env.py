#!/usr/bin/env python3
"""
setup-client-env agent

Usage:
  python3 setup-client-env.py --list-skills
  python3 setup-client-env.py --show-skill ubuntu_install
  python3 setup-client-env.py --apply-skill ubuntu_install --dry-run
  sudo python3 setup-client-env.py --apply-skill ubuntu_install --run

The agent parses skill markdown files under `skills/` and extracts bash code blocks.
It supports dry-run (print commands) and run (execute) modes.
"""

import argparse
import os
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent
SKILLS_DIR = ROOT / "skills"


def find_skills():
    skills = {}
    if not SKILLS_DIR.exists():
        return skills
    for p in SKILLS_DIR.glob("*.md"):
        name = p.stem
        skills[name] = p
    return dict(sorted(skills.items()))


def extract_bash_blocks(md_path: Path):
    text = md_path.read_text(encoding="utf-8")
    blocks = re.findall(r"```bash\n(.*?)```", text, re.S)
    commands = []
    for b in blocks:
        for line in b.splitlines():
            line = line.rstrip()
            if not line:
                continue
            commands.append(line)
    return commands


def show_skill(name, path: Path):
    print(f"Skill: {name}\nFile: {path}\n")
    print(path.read_text(encoding="utf-8"))


def run_commands(cmds, dry_run=True):
    for i, c in enumerate(cmds, 1):
        if dry_run:
            print(f"[DRY-RUN] {i}. {c}")
        else:
            print(f"[RUN] {i}. {c}")
            res = subprocess.run(c, shell=True)
            if res.returncode != 0:
                print(f"Command failed (exit {res.returncode}): {c}")
                sys.exit(res.returncode)


def main():
    parser = argparse.ArgumentParser(description="setup-client-env agent")
    parser.add_argument("--list-skills", action="store_true", help="List available skills")
    parser.add_argument("--show-skill", metavar="SKILL", help="Show skill markdown")
    parser.add_argument("--apply-skill", metavar="SKILL", help="Apply a skill (extract and run its bash blocks)")
    parser.add_argument("--dry-run", action="store_true", help="Print commands without executing")
    parser.add_argument("--run", action="store_true", help="Execute commands (requires privileges for some actions)")
    args = parser.parse_args()

    skills = find_skills()

    if args.list_skills:
        if not skills:
            print("No skills found in skills/ folder.")
            return
        print("Available skills:")
        for k in skills:
            print(f"- {k}")
        return

    if args.show_skill:
        name = args.show_skill
        if name not in skills:
            print(f"Skill not found: {name}")
            sys.exit(1)
        show_skill(name, skills[name])
        return

    if args.apply_skill:
        name = args.apply_skill
        if name not in skills:
            print(f"Skill not found: {name}")
            sys.exit(1)
        cmds = extract_bash_blocks(skills[name])
        if not cmds:
            print("No bash blocks found in skill file.")
            return
        dry = not args.run or args.dry_run
        print(f"Applying skill: {name} (commands: {len(cmds)}) mode={'dry-run' if dry else 'run'})")
        if not dry:
            if os.geteuid() != 0:
                print("Warning: Some commands may require root. Re-run with sudo if needed.")
            print("Proceeding to execute commands...")
        run_commands(cmds, dry_run=dry)
        return

    parser.print_help()


if __name__ == '__main__':
    main()
