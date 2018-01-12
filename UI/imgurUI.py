from flask import Flask
app = Flask(__name__)

# Show main.html
@app.route("/")
def index():
    render_template()

@app.route('/handle_data', method=['POST'])
# Receive the request data
def handleRequestData():
    album_id = request.form['album_id']
    title = request.form['title']
    author = request.form['author']
    use_first_image = request.form['use_first_image']

    
# Used to call the imgurfetch executable to download all of the images to the
# server
# TODO need to implement
def downloadAlbum():
    print("Not Implemented yet")

if __name__ == "__main__":
    app.run()
