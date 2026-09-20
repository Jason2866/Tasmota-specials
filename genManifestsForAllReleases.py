#!/usr/bin/python3

from curses.ascii import isupper
from platform import release
import sys
from os import listdir
from os import mkdir
from os import remove
from os import path
import json
import requests
import gzip


def getManifestsForTag(tag):
    url = "https://github.com/Jason2866/Tasmota-specials/releases/download/" + tag + "/manifests_release.json"
    response = requests.get(url)
    print(url)
    try:
        manifests = json.loads(response.content)
        return manifests
    except:
        return {"Failed":tag}

def getTags():
        tags = []
        with open("tag.txt") as tag:
            tag_latest = tag.readline().strip()
            while tag_latest:
                tags.append(tag_latest)
                tag_latest = tag.readline().strip()
            return tags


def main(args):
    all_tags = getTags()
    all_releases = []
    for tag in all_tags:
        manifests = getManifestsForTag(tag)
        all_releases.append(manifests)

    # print(all_releases)
    j = json.dumps(all_releases,indent=4)
    with gzip.open('all_manifests_in_releases.json.gz', 'wt') as f:
        f.write(j)
        f.close()

if __name__ == '__main__':
  sys.exit(main(sys.argv))
# end if
