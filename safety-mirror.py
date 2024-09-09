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
        to_del = []
        to_reset = []
        for status in command_output(["git", "status", "--porcelain"]).splitlines():
            filename = status[3:].strip()
            {
                "A  ": to_del,
                "DU ": to_del,
                "AA ": to_reset,
                "UU ": to_reset,
            }.get(status[:3], []).append(filename)
        if to_del:
            subprocess.run(["git", "rm", "-f"] + to_del)
        if to_reset:
            subprocess.run(["git", "reset", "--"] + to_reset)
            subprocess.run(["git", "checkout", "--"] + to_reset)
        subprocess.run(["git", "commit"])
