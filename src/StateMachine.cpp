enum leftStickStates {fireTruckRight, fireTruckLeft, fireTruckStraight};
struct LeftStickState
{
    leftStickStates newState;
    leftStickStates oldState;
} truckMotorEscControlStick;
enum rightStickStates {fireTruckForward, fireTruckBackward, fireTruckStill};
struct RightStickState
{
    rightStickStates newState;
    rightStickStates oldState;
} rightStick;

enum fireTruckStates
{
    forward,
    backward,
    right,
    left,
    still,
    forwardRight,
    forwardLeft,
    backwardLeft,
    backwardRight,
};

struct fireTruckState
{
    fireTruckStates newState;
    fireTruckStates oldState;
} fireTruck;

void getState()
{
    truckMotorEscControlStick.newState = setServoState();
    rightStick.newState = setSteeringServoState();
    combineStates(); // will control what gets sent to slave
    sendData();

    truckMotorEscControlStick.oldState = truckMotorEscControlStick.newState;
    rightStick.oldState = rightStick.newState;
}

void combineStates()
{
    if (truckMotorEscControlStick.newState != truckMotorEscControlStick.oldState)
    {
        // truckMotorEscControlStick state is different
        switch (truckMotorEscControlStick.newState)
        {
        case leftStickStates::fireTruckLeft:
            fireTruck.newState = fireTruckStates::left;
            break;
        case leftStickStates::fireTruckRight:
            fireTruck.newState = fireTruckStates::right;
            break;
        default:
            fireTruck.newState = fireTruckStates::still;
            break;
        }
    }

    if (rightStick.newState != rightStick.oldState)
    {
        switch (rightStick.newState)
        {
            // check left stick state 
        case rightStickStates::fireTruckStill:
            // fire truck is still but the left stick is engaged
            if (truckMotorEscControlStick.newState != leftStickStates::fireTruckStraight)
            {
                // left stick is still engaged
                fireTruck.newState = fireTruckStates::right;
                if (truckMotorEscControlStick.newState == leftStickStates::fireTruckLeft)
                {
                    fireTruck.newState = fireTruckStates::left;
                }
            }  else {
                fireTruck.newState == fireTruckStates::still;
            }
            break;
        
        // case for right stick state
        default:
            break;
        }
    }
    
    
}

leftStickStates setServoState()
{
    bool isLeft = leftConditional;
    bool isRight = rightConditional;
    bool isNeutral = !isLeft && !isRight;
    if (isLeft && !isRight)
    {
        truckMotorEscControlStick.newState = leftStickStates::fireTruckLeft;
    } else if (isRight && !isLeft)
    {
        truckMotorEscControlStick.newState = leftStickStates::fireTruckRight;
    } else {
        truckMotorEscControlStick.newState = leftStickStates::fireTruckStraight;
    }
    
    
    // state is different
    if (truckMotorEscControlStick.oldState != truckMotorEscControlStick.newState)
    {
        if (isNeutral) // state is different
        {
            return leftStickStates::fireTruckStraight;
        }
        else if (isRight && !isLeft)
        {
            return leftStickStates::fireTruckRight;
        }
        return leftStickStates::fireTruckLeft;
    }
    return truckMotorEscControlStick.oldState;
}

rightStickStates setSteeringServoState()
{
    bool isForward = forwardConditional;
    bool isBackward = backwardConditional;
    bool isNeutral = !isForward && !isBackward;
    if (isForward && !isBackward)
    {
        rightStick.newState = rightStickStates::fireTruckForward;
    } else if (isBackward && !isForward)
    {
        rightStick.newState = rightStickStates::fireTruckBackward;
    } else {
        rightStick.newState = rightStickStates::fireTruckStill;
    }
    
    
    // state is different
    if (rightStick.oldState != rightStick.newState)
    {
        // right is is neutral
        if (isNeutral)
        {
            return rightStickStates::fireTruckStill;
        }
        else if (isBackward && !isForward)
        {
            return rightStickStates::fireTruckBackward;
        }
        return rightStickStates::fireTruckForward;
    }
    return rightStick.oldState;
}