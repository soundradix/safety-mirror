import json
import subprocess


def command_output(cmd):
    return (
        subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        .stdout.read()
        .decode("utf-8")
    )


remotes = command_output(["git", "remote"]).split()

for repo in json.load(open("sources.json")):
    subprocess.run(
        [
            "git",
            "remote",
            "set-url" if repo["name"] in remotes else "add",
            repo["name"],
            repo["url"],
        ]
    )
    for branch in repo.get("branches", ["main"]):
        subprocess.run(["git", "fetch", repo["name"], branch])
        subprocess.run(
            [
                "git",
                "merge",
                "--allow-unrelated-histories",
                "--no-commit",
                f"""{repo["name"]}/{branch}""",
            ]
        )
        if subprocess.run(["git", "merge", "HEAD"]).returncode == 0:
            # Not merging
            continue
        for status in command_output(["git", "status", "--porcelain"]).splitlines():
            if not status.startswith("A"):
                continue
            filename = status[3:].strip()
            if status.startswith("A  "):
                subprocess.run(["git", "rm", "-f", filename])
            elif status.startswith("AA "):
                subprocess.run(["git", "reset", "--", filename])
                subprocess.run(["git", "checkout", "--", filename])
        subprocess.run(["git", "commit"])
