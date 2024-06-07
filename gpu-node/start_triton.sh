docker build -t triton ./triton
docker run -it --rm --name triton --gpus all -p 8000:8000 -v $PWD/models:/models triton tritonserver --model-repository /models