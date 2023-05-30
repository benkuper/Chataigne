The Github actions build uses a Docker container based on Debian 9 in order to be compatible with older OS versions.  

To build the Docker image, run the following command in this directory : 
`docker build -t chataigne-linux-x64-build .`

Pushing Docker image on the Docker hub : check https://docs.docker.com/get-started/04_sharing_app/

This Docker image is specified in the Github actions `build.yml` : check line `container: tommagnier/chataigne-linux-x64-build`