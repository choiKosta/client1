#!/usr/bin/env python3
"""
setup-client-env agent

Usage:
  python3 setup-client-env.py --list-skills
  python3 setup-client-env.py --show-skill vscode_run_debug
  python3 setup-client-env.py --apply-skill vscode_run_debug --dry-run
  python3 setup-client-env.py --apply-skill vscode_run_debug --run

The agent parses skill markdown files under `skills/` and executes bash code blocks.
It supports dry-run (print commands) and run (execute) modes.
"""

import argparse
import os
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent
PROJECT_ROOT = ROOT.parents[1]
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
    blocks = re.findall(r"```(?:bash|sh)\n(.*?)```", text, re.S)
    return [block.strip() for block in blocks if block.strip()]


def show_skill(name, path: Path):
    print(f"Skill: {name}\nFile: {path}\n")
    print(path.read_text(encoding="utf-8"))


def run_blocks(blocks, dry_run=True):
    env = os.environ.copy()
    env["SETUP_CLIENT_ENV_ROOT"] = str(ROOT)
    env["PROJECT_ROOT"] = str(PROJECT_ROOT)
    for i, block in enumerate(blocks, 1):
        if dry_run:
            print(f"[DRY-RUN] block {i}:")
            print(block)
        else:
            print(f"[RUN] block {i}", flush=True)
            res = subprocess.run(["bash", "-eo", "pipefail", "-c", block], cwd=PROJECT_ROOT, env=env)
            if res.returncode != 0:
                print(f"Skill block failed (exit {res.returncode})")
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
        blocks = extract_bash_blocks(skills[name])
        if not blocks:
            print("No bash blocks found in skill file.")
            return
        dry = not args.run or args.dry_run
        print(f"Applying skill: {name} (bash blocks: {len(blocks)}) mode={'dry-run' if dry else 'run'})", flush=True)
        if not dry:
            if os.geteuid() != 0:
                print("Warning: Some commands may require root. Re-run with sudo if needed.")
            print("Proceeding to execute commands...", flush=True)
        run_blocks(blocks, dry_run=dry)
        return

    parser.print_help()


if __name__ == '__main__':
    main()
