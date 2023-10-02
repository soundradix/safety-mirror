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
                f"""{repo["name"]}/{branch}""",
            ]
        )
        has_conflicts = (
            command_output(["git", "diff", "--name-only", "--diff-filter=U"]) != ""
        )
        subprocess.run(["git", "checkout", "main", "--", "."])
        subprocess.run(["git", "commit"] + ([] if has_conflicts else ["--amend"]))
