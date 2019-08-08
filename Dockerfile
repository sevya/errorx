# Use ubuntu as parent image
FROM ubuntu:18.04

# Set the working directory to /app
WORKDIR /errorx_docker

# Copy the current directory contents into the container at /app
COPY . /errorx_docker

# Install any needed packages specified in requirements.txt
# RUN pip install --trusted-host pypi.python.org -r requirements.txt

# Make port 80 available to the world outside this container
# EXPOSE 80

# Define environment variable
# ENV NAME World
ADD bin/ /
ADD database/ /
ADD internal_data/ /
ADD model.nnet /
ADD optional_file/ / 

ENTRYPOINT ["bin/errorx"]

# Run app.py when the container launches
# CMD ["python", "app.py"]
