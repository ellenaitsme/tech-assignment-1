from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route('/data', methods=['POST'])
def receive_data():
    if request.is_json:
        data = request.json
        print("Received data:")
        print(data)
        
        temperature = data.get('temperature')
        humidity = data.get('humidity')

        response = {
            'message': 'Data received successfully',
            'temperature': temperature,
            'humidity': humidity
        }
        return jsonify(response), 200
    else:
        return jsonify({'error': 'Invalid JSON'}), 400

if __name__ == '__main__':
    app.run(debug=True, host='192.168.0.109', port=5000)
