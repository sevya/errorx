## Runtime calculation

Runtime per 1K sequences in seconds
<table>
	<tr>
    	<th colspan=2>ErrorX + IGBlast</th>
    	<th colspan=2>ErrorX alone</th> 
	</tr>
	<tr>
    	<td>**Single thread**</td>
    	<td>**Multi thread**</td> 
    	<td>**Single thread**</td>
    	<td>**Multi thread**</td> 
	</tr>
	<tr>
		<td>50.9</td>
		<td>15.5</td>
		<td>8.3</td>
		<td>3.3</td>
	</tr>
</table>
	
## False positive/negative calculations
|        | 24407 (human validation) | SRR3175021 (mouse validation) | SRR5676647 (TCR validation) |
|---------------|-------------|--------------|---|
| **TN**        | 3016532     | 1322563      |   |
| **FN**        | 9251        | 1084         |   |
| **FP**        | 2920        | 683          |   |
| **TP**        | 5062        | 1905         |   |
|--------       |-------------|--------------|   |
| **FPR**       | 9.57e-4     | 5.16e-4      |   |
| **Precision** | 0.634       | 0.736        |   |
| **Recall**    | 0.354       | 0.637        |   |

