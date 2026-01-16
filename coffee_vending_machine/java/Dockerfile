# Coffee Vending Machine - Java Dockerfile
# Multi-stage build with Alpine base

# Stage 1: Build stage - compile Java source files
FROM eclipse-temurin:21-jdk-alpine AS builder

WORKDIR /app

# Copy source files
COPY src/ ./src/

# Compile Java files
RUN mkdir -p bin && \
    javac -d bin src/*.java

# Stage 2: Runtime stage - minimal Alpine image with JRE
FROM eclipse-temurin:21-jre-alpine

WORKDIR /app

# Copy compiled classes from builder stage
COPY --from=builder /app/bin/ ./bin/

# Set the working directory to bin for running
WORKDIR /app/bin

# Run the coffee vending machine demo
CMD ["java", "CoffeeVendingMachineDemo"]
