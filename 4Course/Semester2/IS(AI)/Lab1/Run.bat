@echo off
	rem Batch file to start lab1 code
	python -m pip install --upgrade pip
echo installing Flask
	pip install flask
echo installed Flask
echo installing Yaml
	pip install PyYAML
echo installed Yaml
echo installing ChatterBot
	pip install chatterbot
echo installed ChatterBot
echo installing ChatterBot corpus
	pip install chatterbot-corpus
echo installed ChatterBot corpus
echo Running App
	python app.py
	
pause