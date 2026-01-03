# Dockerfile 示例
# 展示 Dockerfile 的语法特性

# 使用官方的基础镜像
FROM ubuntu:20.04

# 设置构建参数
ARG USERNAME=developer
ARG USER_UID=1000
ARG USER_GID=$USER_UID

# 设置环境变量
ENV DEBIAN_FRONTEND=noninteractive
ENV LANG=C.UTF-8
ENV LC_ALL=C.UTF-8

# 设置工作目录
WORKDIR /app

# 复制文件
COPY . /app/

# 安装系统依赖
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    python3 \
    python3-pip \
    nodejs \
    npm \
    ruby \
    php \
    && rm -rf /var/lib/apt/lists/*

# 创建非root用户
RUN groupadd --gid $USER_GID $USERNAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME \
    && chown -R $USERNAME:$USERNAME /app

# 切换到非root用户
USER $USERNAME

# 设置Python虚拟环境
RUN python3 -m venv /home/$USERNAME/venv
ENV PATH="/home/$USERNAME/venv/bin:$PATH"

# 安装Python依赖
RUN pip install --upgrade pip
COPY requirements.txt .
RUN pip install -r requirements.txt

# 安装Node.js依赖
COPY package*.json ./
RUN npm install

# 安装Ruby gems
COPY Gemfile Gemfile.lock ./
RUN bundle install

# 复制应用代码
COPY . .

# 构建应用
RUN make build

# 暴露端口
EXPOSE 3000 8080

# 设置健康检查
HEALTHCHECK --interval=30s --timeout=3s --start-period=5s --retries=3 \
    CMD curl -f http://localhost:3000/health || exit 1

# 设置卷挂载点
VOLUME ["/app/data", "/app/logs"]

# 设置默认命令
CMD ["npm", "start"]

# 使用多阶段构建的示例
FROM node:16-alpine AS builder

WORKDIR /build

COPY package*.json ./
RUN npm ci --only=production

FROM node:16-alpine AS runtime

WORKDIR /app

# 从构建阶段复制文件
COPY --from=builder /build/node_modules ./node_modules
COPY . .

USER node

EXPOSE 3000

CMD ["node", "server.js"]
