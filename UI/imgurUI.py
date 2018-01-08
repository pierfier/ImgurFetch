from flask import Flask
app = Flask(__name__)

# Show the main page
@app.route("/")
def index():
    render_template()

if __name__ == "__main__":
    app.run()
