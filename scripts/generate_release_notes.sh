#!/bin/bash

# 生成发布说明脚本
# 用法: ./generate_release_notes.sh <VERSION> [PREV_VERSION]

set -e

VERSION=$1
PREV_VERSION=${2:-"latest"}
DATE=$(date +%Y-%m-%d)
USERNAME=${GITHUB_REPOSITORY_OWNER:-"your-username"}

if [ -z "$VERSION" ]; then
    echo "错误: 请提供版本号"
    echo "用法: $0 <VERSION> [PREV_VERSION]"
    exit 1
fi

# 创建临时发布说明文件
TEMP_RELEASE_FILE="release_notes_${VERSION}.md"

# 替换模板中的占位符
sed -e "s/{VERSION}/${VERSION}/g" \
    -e "s/{DATE}/${DATE}/g" \
    -e "s/{USERNAME}/${USERNAME}/g" \
    -e "s/{PREV_VERSION}/${PREV_VERSION}/g" \
    -e "s|{PREV_RELEASE_URL}|https://github.com/${USERNAME}/pnana/releases/tag/v${PREV_VERSION}|g" \
    RELEASE.md > "${TEMP_RELEASE_FILE}"

echo "已生成发布说明: ${TEMP_RELEASE_FILE}"
echo "请编辑此文件，添加实际的版本更新内容，然后用于 GitHub Release"

# 如果是 GitHub Actions 环境，输出文件路径
if [ -n "$GITHUB_ACTIONS" ]; then
    echo "release_notes_file=${TEMP_RELEASE_FILE}" >> $GITHUB_OUTPUT
fi