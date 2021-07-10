import csv
import numpy as np
import matplotlib.pyplot as plt

# Input filename in .csv format
# Returns:
# - time since Arduino came online ( time [sec] )
# - relative humidity ( rel_hum [%] )
# - temperature ( temp [degC] )
# - pressure ( pres [Pa] )
def Parse_Data( filename , time_top ):

    rt = open( filename , "r" )

    if rt.readable():
        data = csv.reader( rt )
        lst = []

        for line in data:
            lst.append( line )

        tnow = 0.0
        j = 0
        while j < ( len( lst ) - 1 ):
            tnow = float( lst[ j + 1 ][ 0 ] )
            if tnow < time_top*1000.0:
                j += 1
            else:
                break
                
        ndata = j - 1

        time = np.zeros( ndata )
        rel_hum = np.zeros( ndata )
        temp = np.zeros( ndata )
        pres = np.zeros( ndata )

        for i in range( 0 , ndata ):
            time[ i ] = float( lst[ i + 1 ][ 0 ] )/1000.0
            rel_hum[ i ] = float( lst[ i + 1 ][ 1 ] )
            temp[ i ] = float( lst[ i + 1 ][ 2 ] )
            pres[ i ] = float( lst[ i + 1 ][ 3 ] )
    else:
        time = []
        rel_hum = []
        temp = []
        pres = []
        print( "Unreadable data, returning empty arrays!" )
    
    return time , rel_hum , temp , pres

# Filename for the plot
file_name = "Flights/FLIGHT_1.csv" 
#file_name = "Flights/FLIGHT_2.csv" # Filename for the plot

time_top = 427 # Time at which the maximum height has been reached in [sec]

time , rel_hum , temp , pres = Parse_Data( file_name , time_top )

fig = plt.figure( )
ax1 = plt.subplot( 311 )

# Plot Pressure vs. Time
ax1.plot( time , pres , color = "red" , linestyle = "solid" )

ax2 = plt.subplot( 312 , sharex = ax1 )

# Plot Humidity vs. Time
ax2.plot( time , rel_hum , color = "green" , linestyle = "solid" )

ax3 = plt.subplot( 313 , sharex = ax1 )

# Plot Temperature vs. Time
ax3.plot( time , temp , color = "blue" , linestyle = "solid" )

# Set Labels where needed
ax1.set_xlabel( r"Time since Start [sec]" )
ax1.set_ylabel( r"Pressure [Pa]" )
ax2.set_ylabel( r"Humidity [%]" )
ax3.set_ylabel( r"Temperature [˚C]" )

# Save the figure if you want
#plt.savefig( "Time_Dependent_Values.eps", format="eps" )

plt.show()

fig = plt.figure( )
ax1 = plt.subplot( )

# Plot Pressure vs. Temperature
ax1.plot( pres , temp , color = "blue" , linestyle = "solid" )

# Set Labels where needed
ax1.set_xlabel( r"Pressure [Pa]" )
ax1.set_ylabel( r"Temperature [˚C]" )

# Save the figure if you want
plt.savefig( "P_T_Plot.eps", format="eps" )

plt.show()