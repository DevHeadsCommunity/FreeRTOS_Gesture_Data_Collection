import serial 
import csv
import statistics

# Initialize containers to store data for calculations
data_storage = {
    "Ax": [],
    "Ay": [],
    "Az": [],
    "Gx": [],
    "Gy": [],
    "Gz": []
}

final_data = ['Not Circle']

# Open/Create an output .csv file and add headers
with open('output.csv', 'a+', newline='') as csvfile:
    output = csv.writer(csvfile)
    # output.writerow(['Gesture', 'Ax', 'Ay', 'Az', 'Gx', 'Gy', 'Gz'])  # Add appropriate headers

    # Configure and open the serial port
    try:
        ser = serial.Serial('/dev/ttyACM0', baudrate=115200, timeout=1)  # Added timeout for readline
        print(f"Connected to: {ser.name}")  # Print the name of the serial port

        while True:
                # Read a line of data from the serial port
                data = ser.readline().decode('utf-8').strip()  # Strip whitespace/newlines
                if data:  # Check if data is not empty
                    values = data.split(' ')  # Split data assuming space-separated values
                    
                    # Ensure the data has the correct number of elements before writing
                    if len(values) == 6:  # Expecting Ax, Ay, Az, Gx, Gy, Gz
                        # Convert strings to floats for calculations
                        float_values = [float(v) for v in values]
                        
                        # Write to CSV
                        # output.writerow(float_values)
                        print(float_values)  # Optionally print to console for debugging

                        # Add data to storage for calculations
                        data_storage["Ax"].append(float_values[0])
                        data_storage["Ay"].append(float_values[1])
                        data_storage["Az"].append(float_values[2])
                        data_storage["Gx"].append(float_values[3])
                        data_storage["Gy"].append(float_values[4])
                        data_storage["Gz"].append(float_values[5])
                    else:
                        print(f"Invalid data received: {data}")  # Debugging information for invalid data

    
    except serial.SerialException as e:
        print(f"Serial port error: {e}")
    
    except KeyboardInterrupt:
        print("\nExiting...")
        # Calculate mean and SD when exiting
        print("\nCalculating statistics...")
        for key, values in data_storage.items():
            if values:  # Ensure there is data to calculate
                mean = statistics.mean(values)
                stdev = statistics.stdev(values) if len(values) > 1 else 0  # SD needs at least 2 data points
                final_data.append(stdev/mean) # adding final calcualte data into the list so that we can write into csv 
                print(f"{key}: Mean = {mean:.4f}, SD = {stdev:.4f}")
            else:
                print(f"{key}: No data available.")
        output.writerow(final_data)  #Writing Data into CSV
        print("Data is Written into .csv")
    
    finally:
        ser.close()  # Close the serial port when done
