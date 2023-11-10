#!/usr/bin/bash

# Load the ANS value from the file, if it exists
# If not exist, ANS = 0
if [[ -f "ans.txt" ]]; then
    res=$(cat ans.txt)
    rm -rf history.txt
fi

# Save valid math to history.txt file
save_history() {
    # Save the ANS value to the file
    echo $res > ans.txt

    if $a_as_ans; then
        echo "ANS" $operator $b = $res >> history.txt
    elif $b_as_ans; then
        echo $a $operator "ANS" = $res >> history.txt
    else
        echo $a $operator $b = $res >> history.txt
    fi

    line_count=$(wc -l < history.txt)

    # Keep the file line = 5
    if [ "$line_count" -gt "5" ]; then
        sed -i '1d' history.txt
    fi
}

while read -p ">> " a operator b && [ "$a" != "EXIT" ]; do
    # Any valid math will save to history
    valid_math=true
    is_math_operation=true
    a_as_ans=false
    b_as_ans=false

    # CHECK IF THE INPUT IS NUMBER OR NOT
    if ! [[ $a =~ ^[0-9]+$ ]] || ! [[ $b =~ ^[0-9]+$ ]]; then
        if [[ $a == "ANS" ]]; then
            a=$res
            a_as_ans=true
        elif [[ $b == "ANS" ]]; then
            b=$res
            b_as_ans=true
        elif [[ $a == "HISTORY" ]]; then
            cat history.txt
            valid_math=false
            is_math_operation=false
        else
            echo "MATH ERROR"
            valid_math=false
            is_math_operation=false
        fi
    fi

    if $is_math_operation; then
        # Handle operator from user input
        case $operator in
        +)
            res=`echo $a + $b | bc`
        ;;
        -)
            res=`echo $a - $b | bc`
        ;;
        /)
            if [ $b == "0" ]; then
                res=`echo "MATH ERROR"`
                valid_math=false
            else
                res=`echo "scale=2; $a / $b" | bc`
            fi
        ;;
        x)
            res=`echo $a \* $b | bc`
        ;;
        %)
            res=`echo $a % $b | bc`
        ;;
        *)
            res=`echo "SYNTAX ERROR"`
            valid_math=false
        ;;
        esac
        # Print the result to screen
        echo $res
    fi

    # Save valid math
    if $valid_math; then
        save_history
    fi

    echo -----------------------------------------
    read -n 1 -s -r -p "Press any key to continue"
    clear
done
